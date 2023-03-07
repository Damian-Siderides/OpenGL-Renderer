#pragma once

#include <string>
#include <GL/glew.h>

#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VertexBufferLayout.h>
#include <VertexArray.h>

#include <optional>

class Object {
public:
	// std::string name;
	// std::string material;

	// probably need to malloc?
	// char* name;
	// char* material;

	// VertexBuffer vb_3D;
	// VertexArray va_3D;
	// VertexBufferLayout layout_3D;
	// IndexBuffer ib_3D;

	unsigned int vb_3D_size = 24;
	unsigned int ib_3D_size = 36;
	unsigned int vb_2D_size = 8;
	unsigned int ib_2D_size = 6;

	VertexArray va_3D{};
	VertexBufferLayout layout_3D{};

	VertexArray va_2D{};
	VertexBufferLayout layout_2D{};
	

	char name[128] = "Object Name Here";
	char material[128] = "Wood";
	int id;

	float x, y, z; // x y z Coordinates in 3D Space
	float i, j, k; // rotation

	float height, width, depth;

	

	GLfloat vb_3D_data[24] = {}; // = {
 //  		-width/2,  height/2,  depth/2, // 0		bottom left front
 //  		 width/2, -height/2,  depth/2, // 1		bottom right front
 //  		-width/2,  height/2,  depth/2, // 2 	top left front
 //  		 width/2,  height/2,  depth/2, // 3		top right front
 //  		-width/2, -height/2, -depth/2, // 4 	bottom left back
 //  		 width/2, -height/2, -depth/2, // 5 	bottom right back
 //  		-width/2,  height/2, -depth/2, // 6 	top left back
 //  		 width/2,  height/2, -depth/2  // 7 	top right back
	// };
	// Index Buffer 3D
	unsigned int ib_3D_data[36] = {
		0, 1, 3, // front
		0, 2, 3,
		1, 5, 3, // right
		5, 3, 7,
		0, 4, 2, // left
		4, 2, 6,
		4, 5, 7, // back
		4, 6, 7,
		2, 3, 6, // top
		3, 6, 7,
		0, 1, 4, // bottom
		1, 4, 5
	};

	// Vertex Buffer Plan View
	GLfloat vb_2D_data[8] = {}; // = {
 //  		-width, -depth, // 0
 //  		 width, -depth, // 1
 //  		 width,  depth, // 2
 //  	   	-width,  depth  // 3
	// };

	// Index  Buffer Plan View
	unsigned int ib_2D_data[6] = {
  		0, 1, 2,
  		2, 3, 0
	};

	std::optional<VertexBuffer> vb_3D;
	std::optional<IndexBuffer>  ib_3D;

	std::optional<VertexBuffer> vb_2D;
	std::optional<IndexBuffer>  ib_2D;

	Object();	
	Object(float height, float width, float depth);
	~Object();

	void update_dimensions();
};

GLfloat* polygon(int n);
GLfloat* color(int n);
unsigned int* index(int n);