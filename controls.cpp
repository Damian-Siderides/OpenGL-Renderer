// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.
extern bool show_Object_Menu2;
extern bool show_Object_List;
extern bool show_Object_Menu;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include <iostream>
#include <stdio.h>
// #include "gui.cpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}

glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

/*
	https://www.glfw.org/docs/latest/input_guide.html
*/

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 800 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
float FoV = 45.0f;

float speed = 300.0f; // 30 units / second
float mouseSpeed = 0.005f;

double xpos_LastFrame, ypos_LastFrame;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// std::cout << xoffset << " : " << yoffset << std::endl;
	// printf("%f : %f\n", xoffset, yoffset);
	// printf("FOV: %f\n", FoV);

	FoV -= yoffset;
}

extern bool is3D;

void computeMatricesFromInputs(){


	// unlimited mouse movement? - might provide locking to window and hide cursor
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// hide cursor
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// return to normal
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion

	if (is3D == false) {
		horizontalAngle = 3.14f;
		verticalAngle   = 0.0f;
	}
	
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	

	// Up vector
	glm::vec3 up = glm::cross( right, direction );


	if (glfwGetKey( window, GLFW_KEY_E) == GLFW_PRESS) {
			is3D = true;
	}

	if (glfwGetKey( window, GLFW_KEY_R) == GLFW_PRESS) {
			is3D = false;
	}

	// Reset mouse position for next frame

	// Compute new orientation
	if (is3D && (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ) {
		glfwSetCursorPos(window, 1024/2, 768/2);
		horizontalAngle += mouseSpeed * float(1024/2 - xpos );
		verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	} else {

		if (glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {

			// position += right * mouseSpeed * float(1024/2 - xpos );
			// position -= up * mouseSpeed * float( 768/2 - ypos );

			position += right * mouseSpeed * 100.0f * float(xpos_LastFrame - xpos);
			position -= up    * mouseSpeed * 100.0f * float(ypos_LastFrame - ypos);
		}
	}

	if (glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		// CabinetMenu();
		show_Object_Menu2 = true;
	}
	if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS) {
		show_Object_List = true;
	}
	if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS) {
		show_Object_Menu2 = true;
	}
	if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS) {
		show_Object_Menu = true;
	}
	
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS) {
		position += up * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS) {
		position -= up * deltaTime * speed;
	}

	// FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// glfwSetScrollCallback( window, scroll_callback);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 10000.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

	xpos_LastFrame = xpos;
	ypos_LastFrame = ypos;

}