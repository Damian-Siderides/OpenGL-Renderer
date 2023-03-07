#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <GL/glew.h>

#include <stdlib.h>
#include <math.h>

#include "object.hpp"

#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VertexBufferLayout.h>
#include <VertexArray.h>

using namespace std;

std::vector<Object> objects;

Object::Object() 
// :vb_2D(vb_2D_data, vb_2D_size * sizeof(float)), ib_2D(ib_2D_data, ib_2D_size) {
:Object(500, 500, 500)
{
	// name = "Object Name Here";
	// material = "Wood";
	// id = objects.size() + 1;
	
	// x = y = z = 0;
	// i = j = k = 0;

	// height = width = depth = 500;

	// update_dimensions();

	// // 3D - possibly have to rebind vertex buffer, everytime dimensions change?
	// // figure it out later
	// // vb_3D = VertexBuffer(vb_3D_data, vb_3D_size * sizeof(float));
	
	// // layout_3D.Push(3);
	// // va_3D.AddBuffer(vb_3D, layout_3D);

	// // ib_3D = IndexBuffer(ib_3D_data, ib_3D_size);

	// // 2D
	// // vb_2D = new VertexBuffer(vb_2D_data, vb_2D_size * sizeof(float));
	
	// layout_2D.Push(2);
	// va_2D.AddBuffer(vb_2D, layout_2D);

	// printf("VA Address: %p\n", &va_2D);
	// printf("IB Address: %p\n", &ib_2D);


	// ib_2D = new IndexBuffer(ib_2D_data, ib_2D_size);

	// objects.vector::push_back(*this);
}
	
Object::Object(float height, float width, float depth)
// :vb_2D(vb_2D_data, vb_2D_size * sizeof(float)), ib_2D(ib_2D_data, ib_2D_size) {
{
	// name = "Object Name Here";
	// material = "Wood";

	id   = objects.size() + 1;

	x = y = z = 0;
	i = j = k = 0;

	this->height = height;
	this->width  = width;
	this->depth  = depth;

	// for loop to do vertex buffers, height width depth etc.

	update_dimensions();

	// 3D - possibly have to rebind vertex buffer, everytime dimensions change?
	// figure it out later
	// vb_3D = VertexBuffer(vb_3D_data, vb_3D_size * sizeof(float));
	
	// layout_3D.Push(3);
	// va_3D.AddBuffer(vb_3D, layout_3D);

	// ib_3D = IndexBuffer(ib_3D_data, ib_3D_size);

	// 2D
	// vb_2D = new VertexBuffer(vb_2D_data, vb_2D_size * sizeof(float));
	
	// 3D
	vb_3D.emplace(vb_3D_data, vb_3D_size * sizeof(float));
	
	layout_3D.Push(3);
	va_3D.AddBuffer(*vb_3D, layout_3D);

	ib_3D.emplace(ib_3D_data, ib_3D_size);

	// 2D
	vb_2D.emplace(vb_2D_data, vb_2D_size * sizeof(float));
	
	layout_2D.Push(2);
	va_2D.AddBuffer(*vb_2D, layout_2D);

	ib_2D.emplace(ib_2D_data, ib_2D_size);

	ib_2D.emplace(ib_2D_data, ib_2D_size);

	// objects.push_back(*this);

}
	//~Object();
Object::~Object() {
	// objects.remove()

	// delete this;
}

void Object::update_dimensions() {

	// vertex_buffer_3D[24] = {
//   		-width/2, -height/2,  depth/2, // 0		bottom left front
//   		 width/2, -height/2,  depth/2, // 1		bottom right front
//   		-width/2,  height/2,  depth/2, // 2 	top left front
//   		 width/2,  height/2,  depth/2, // 3		top right front
//   		-width/2, -height/2, -depth/2, // 4 	bottom left back
//   		 width/2, -height/2, -depth/2, // 5 	bottom right back
//   		-width/2,  height/2, -depth/2, // 6 	top left back
//   		 width/2,  height/2, -depth/2  // 7 	top right back
	// };
	
	vb_3D_data[0]  = -width/2;
	vb_3D_data[1]  = -height/2;
	vb_3D_data[2]  =  depth/2;

	vb_3D_data[3]  =  width/2;
	vb_3D_data[4]  = -height/2;
	vb_3D_data[5]  =  depth/2;

	vb_3D_data[6]  = -width/2;
	vb_3D_data[7]  =  height/2;
	vb_3D_data[8]  =  depth/2;

	vb_3D_data[9]  =  width/2;
	vb_3D_data[10] =  height/2;
	vb_3D_data[11] =  depth/2;

	vb_3D_data[12] = -width/2;
	vb_3D_data[13] = -height/2;
	vb_3D_data[14] = -depth/2;

	vb_3D_data[15] =  width/2;
	vb_3D_data[16] = -height/2;
	vb_3D_data[17] = -depth/2;

	vb_3D_data[18] = -width/2;
	vb_3D_data[19] =  height/2;
	vb_3D_data[20] = -depth/2;

	vb_3D_data[21] =  width/2;
	vb_3D_data[22] =  height/2;
	vb_3D_data[23] = -depth/2;

	vb_2D_data[0] = -width/2;
	vb_2D_data[1] = -depth/2;
	vb_2D_data[2] =  width/2;
	vb_2D_data[3] = -depth/2;
	vb_2D_data[4] =  width/2;
	vb_2D_data[5] =  depth/2;
	vb_2D_data[6] = -width/2;
	vb_2D_data[7] =  depth/2;
	
}

/*
TODO
Circle Fan
*/

GLfloat* polygon(int n) {
	// float degrees = (n - 2.0f) * 180.0f / n;

	float degrees = (n - 2.0f) * M_PI / n;

	printf("Degrees = %f\n", degrees);

	// float* positions = (float*)malloc(2 * n * sizeof(float));
	float* positions = new float[2 * n];

	for (int i = 0; i < 2 * n; i += 2) {
		positions[i] 	 = cos(i * degrees);
		positions[i + 1] = sin(i * degrees);
	}

	// printf("%s\n", "Positions");

	// Print Section

	// for (int i = 0; i < 2 * n; i += 2) {
	// 	printf("(%f, %f)\n", positions[i], positions[i + 1]);
	// }
	return positions;
}

GLfloat* color(int n) {

	// float* positions = (float*)malloc(2 * n * sizeof(float));
	float* colors = new float[n];

	time_t t;

	srand((unsigned) time(&t));

	for (int i = 0; i < n; i++) {
		colors[i] = ((float) rand() / (float) RAND_MAX);
		// colors[i] = 1;
	}

	// for (int i = 0; i < n; i++) {
	// 	printf("Color %d: %f\n", i + 1, colors[i]);
	// }
	return colors;
}

unsigned int* index(int n) {
	// int* indices = (int*) malloc(3 * (n - 2) * sizeof(int));
	unsigned int* indices = new unsigned int[3 * (n - 2)];

	for (int i = 0, j = 0; i < n; i += 3, j++) {
		indices[i] 	   = 0;
		indices[i + 1] = j + 1;
		indices[i + 2] = j + 2;
	}

	// printf("%s\n", "Indices");

	// can this for loop be done with only one variable?
	// Print Section
	// for (int i = 0; i < n; i += 3) {
	// 	printf("(%d, %d, %d)\n", indices[i], indices[i + 1], indices[i + 2]);
	// }
	return indices;
}


// printf("%s %ld\n", "Size of float: \n", sizeof(float));
// 	printf("%s %ld\n", "Size of unsigned int: \n" , sizeof(unsigned int));

	// Print Statements

	// Print Section
	// printf("%s\n", "Positions");

	// printf("%s %ld\n", "Size of Positions is: ", sizeof(positions));

	// for (int i = 0; i < 2 * n; i += 2) {
	// 	printf("(%f, %f)\n", positions[i], positions[i + 1]);
	// }

	// printf("%s\n", "Indices");

	// printf("%s %ld\n", "Size of Indices is: ", sizeof(indices));

	// // Print Section
	// for (int i = 0; i < n; i += 3) {
	// 	printf("(%d, %d, %d)\n", indices[i], indices[i + 1], indices[i + 2]);
	// }













// int main() {

// 	//unique_ptr

// 	Object *x = new Object();

// 	unique_ptr<Object> y = make_unique<Object>();

// }

class Position {
public:
	double x, y, z;
	double i, j, k;

	Position() {
		x = y = z = 0;
		i = j = k = 0;
	}
};
