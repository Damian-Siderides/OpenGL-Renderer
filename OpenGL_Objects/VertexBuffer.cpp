#include "VertexBuffer.h"
// #include <stdio.h>
#include <Renderer.h>

VertexBuffer::VertexBuffer() {
	
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


// class VertexBuffer {
// private:
// 	unsigned int m_RendererID;

// public:
// 	GLuint vertexbuffer;

// 	VertexBuffer(const void* data, unsigned int size);

// 	void Bind();
// 	void Unbind();

// 	VertexBuffer() {
		
// 	}



// };