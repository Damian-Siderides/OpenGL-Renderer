

// Include GLEW
#include <GL/glew.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "object.hpp"

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

extern std::vector<Object> objects;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

#include "shader.hpp"
#include "controls.hpp"
#include <glm/gtc/matrix_transform.hpp>


#include <time.h>

#define M_PI           3.14159265358979323846

#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <ColorBuffer.h>
#include <VertexArray.h>
#include <VertexBufferLayout.h>

#include "gui.h"
// #include "gui.cpp"

#define ASSERT(x) if (!(x)) 

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static bool GLCheckError(const char* function, const char* file, int line) {
	while(GLenum error = glGetError()) {
		std::cout << "[OpenGl Error] (" << error << "): " << function << " " << file << ": Line " << line << std::endl;
		return false;
	}
	return true;
}




bool show_Object_Menu2 = true;
bool show_Object_List = true;
bool show_Object_Menu = true;
extern bool open_button;

bool is3D = true;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

glm::mat4 mvp;
// glm::mat4 ProjectionMatrix;
// glm::mat4 ViewMatrix;
// glm::mat4 ModelMatrix;

// void MatrixStuff(bool is3D) {
// void MatrixStuff(bool is3D, Object x) {
void MatrixStuff(float x, float y, float z) {
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	// glm::mat4 ModelMatrix = glm::mat4(1.0);
	// why Object doesn't work, unknown, possibly pointer issue.
	// glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(object.x, object.y, object.z));
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));

	mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
}

void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
){

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)mouseY/(float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
		((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
	);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);
	
	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);
	
	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
	glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
	glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w;
	glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}


bool TestRayOBBIntersection(
	glm::vec3 ray_origin,        // Ray origin, in world space
	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
){
	
	// Intersection method from Real-Time Rendering and Essential Mathematics for Games
	
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if ( fabs(f) > 0.001f ){ // Standard case

			float t1 = (e+aabb_min.x)/f; // Intersection with the "left" plane
			float t2 = (e+aabb_max.x)/f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2
			if (t1>t2){
				float w=t1;t1=t2;t2=w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if ( t2 < tMax )
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if ( t1 > tMin )
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin )
				return false;

		}else{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if(-e+aabb_min.x > 0.0f || -e+aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray_direction, yaxis);

		if ( fabs(f) > 0.001f ){

			float t1 = (e+aabb_min.y)/f;
			float t2 = (e+aabb_max.y)/f;

			if (t1>t2){float w=t1;t1=t2;t2=w;}

			if ( t2 < tMax )
				tMax = t2;
			if ( t1 > tMin )
				tMin = t1;
			if (tMin > tMax)
				return false;

		}else{
			if(-e+aabb_min.y > 0.0f || -e+aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray_direction, zaxis);

		if ( fabs(f) > 0.001f ){

			float t1 = (e+aabb_min.z)/f;
			float t2 = (e+aabb_max.z)/f;

			if (t1>t2){float w=t1;t1=t2;t2=w;}

			if ( t2 < tMax )
				tMax = t2;
			if ( t1 > tMin )
				tMin = t1;
			if (tMin > tMax)
				return false;

		}else{
			if(-e+aabb_min.z > 0.0f || -e+aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;

}

int main( void )
{
	// Initialise GLFW
	// if( !glfwInit() )
	// {
	// 	fprintf( stderr, "Failed to initialize GLFW\n" );
	// 	getchar();
	// 	return -1;
	// }

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

	#if defined(IMGUI_IMPL_OPENGL_ES2)
	    // GL ES 2.0 + GLSL 100
	    const char* glsl_version = "#version 100";
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(__APPLE__)
	    // GL 3.2 + GLSL 150
	    const char* glsl_version = "#version 150";
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
	#else
	    // GL 3.0 + GLSL 130
	    const char* glsl_version = "#version 130";
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1000, 1000, "Open Kitchens", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// glfwSwapInterval(1);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// float dimension = 500.0;

	// Object object = Object(dimension, 300, dimension);
	// Object object2 = Object(dimension, 300, dimension);

	// printf("Object Created\n");

	// printf("Index Buffer 2D\n");
	// for (int i = 0; i < object.ib_2D_size; i++) {
	// 	printf("%d: %d\n", i, object.ib_2D_data[i]);
	// }

	// printf("Index Buffer 3D\n");
	// for (int i = 0; i < object.ib_3D_size; i++) {
	// 	printf("%d: %d\n", i, object.ib_3D_data[i]);
	// }

	// printf("Vertex Buffer 2D\n");
	// for (int i = 0; i < object.vb_2D_size; i++) {
	// 	printf("%d: %f\n", i, object.vb_2D_data[i]);
	// }

	// printf("Vertex Buffer 3D\n");
	// for (int i = 0; i < object.vb_3D_size; i++) {
	// 	printf("%d: %f\n", i, object.vb_3D_data[i]);
	// }

	// GLfloat* colour = color(n);
	
	// Class Way
	// VertexBuffer vb;
	// VertexArray va;
	// VertexBufferLayout layout;
	// IndexBuffer ib;

	// // don't know why, if you dynamically allocate, code will work
	// vb = VertexBuffer(object.vb_2D_data, object.vb_2D_size * sizeof(float));
	
	// layout.Push(2);
	// va.AddBuffer(*vb, layout);

	// ib = IndexBuffer(object.ib_2D_data, object.ib_2D_size);

	// Normal Way

	// VertexBuffer vb(object.vb_2D_data, object.vb_2D_size * sizeof(float));
	
	// VertexArray va;
	// VertexBufferLayout layout;

	// layout.Push(2);
	// va.AddBuffer(vb, layout);

	// IndexBuffer ib(object.ib_2D_data, object.ib_2D_size);

	Renderer rend;

	// printf("Renderer Created\n");



	// ColorBuffer c(colour, 20 * sizeof(float));

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );
	// GLuint programID = LoadShaders( "NewVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	// GLuint programID = LoadShaders("ChernoVertexShader.vs", "ChernoFragmentShader.fs");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Dark blue background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	int x_counter = 0;

	do {

		// Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

 		// glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        MainMenuBar();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        if (show_Object_Menu2) {
        	ObjectMenu2();
        }
        if (show_Object_List) {
        	ObjectList(&object);
        }
        if (show_Object_Menu) {
        	ObjectMenu();
        }

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT ); // with 3D, this line missing causes weird effect
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// MatrixStuff(false, object.at(0));
		// MatrixStuff(false);
		// MatrixStuff(true);

		


		// // PICKING IS DONE HERE
		// // (Instead of picking each frame if the mouse button is down, 
		// // you should probably only check if the mouse button was just released)
		// if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
			
		// 	glm::vec3 ray_origin;
		// 	glm::vec3 ray_direction;
		// 	ScreenPosToWorldRay(
		// 		1024/2, 768/2,
		// 		1024, 768, 
		// 		getViewMatrix(), 
		// 		getProjectionMatrix(), 
		// 		ray_origin, 
		// 		ray_direction
		// 	);	
			
		// 	//ray_direction = ray_direction*20.0f;

		// 	// message = "background";

		// 	// Test each each Oriented Bounding Box (OBB).
		// 	// A physics engine can be much smarter than this, 
		// 	// because it already has some spatial partitionning structure, 
		// 	// like Binary Space Partitionning Tree (BSP-Tree),
		// 	// Bounding Volume Hierarchy (BVH) or other.


		// 	for(int i=0; i<100; i++){

		// 		float intersection_distance; // Output of TestRayOBBIntersection()
		// 		glm::vec3 aabb_min(-1.0f, -1.0f, -1.0f);
		// 		glm::vec3 aabb_max( 1.0f,  1.0f,  1.0f);

		// 		// The ModelMatrix transforms :
		// 		// - the mesh to its desired position and orientation
		// 		// - but also the AABB (defined with aabb_min and aabb_max) into an OBB
		// 		// glm::mat4 RotationMatrix = glm::toMat4(orientations[i]);
		// 		// glm::mat4 TranslationMatrix = translate(mat4(), positions[i]);
		// 		glm::mat4 RotationMatrix = glm::toMat4(glm::quat( glm::vec3(0, 0, 0) ));
		// 		glm::mat4 TranslationMatrix = translate(mat4(), glm::vec3(0, 0, 0));
		// 		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix;

		// 		if ( TestRayOBBIntersection(
		// 			ray_origin, 
		// 			ray_direction, 
		// 			aabb_min, 
		// 			aabb_max,
		// 			ModelMatrix,
		// 			intersection_distance)
		// 		){
		// 			x_counter++;
		// 			// std::ostringstream oss;
		// 			// oss << "mesh " << i;
		// 			// message = oss.str();
		// 			printf("%d: Interestion?\n", x_counter);
		// 			break;
		// 		}
		// 	}


		// }


    	//glEnable(GL_CULL_FACE);

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		
		// glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		glPolygonMode(GL_FRONT, GL_LINE);

		glUseProgram(programID);
		
		int location = glGetUniformLocation(programID, "u_Color");

		glUniform4f(location, 0.5f, 0.2f, 0.2f, 1.0f);

		// colour = color(n);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n, colour, GL_STATIC_DRAW);

		// Draw the triangle !

		for (unsigned int i = 0; i < object.size(); i++) {
		
			if (objects.size() == 0) {
				break;
			}
		// 	// render said object
		// 	// object[i].update_dimensions();
			// MatrixStuff(false, object.at(i).x, object.at(i).y, object.at(i).z);
			MatrixStuff(object.at(i).x, object.at(i).y, object.at(i).z);
			// MatrixStuff(false, object.at(i));
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

			if (is3D) {
				GLCall(rend.Draw(object.at(i).va_3D, *(object.at(i).ib_3D) ));
			} else {
				GLCall(rend.Draw(object.at(i).va_2D, *(object.at(i).ib_2D) ));
			}
		// 	// rend.Draw(object.at(i).va_2D, (object.at(i).ib_2D) );
		// 	// rend.Draw(va, ib);
		}

		// rend.Draw(object.va_2D, *object.ib_2D);
		// rend.Draw(object2.va_2D, *object2.ib_2D);

		// GLCall(rend.Draw(object.va_2D, *(object.ib_2D)));
		// GLCall(rend.Draw(object->va_3D, object->ib_3D));
		// GLCall(rend.Draw(va, ib));
		
		ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

