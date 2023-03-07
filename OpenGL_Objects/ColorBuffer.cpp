#include "ColorBuffer.h"

// #include "Renderer.h"

ColorBuffer::ColorBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
}

ColorBuffer::~ColorBuffer() {
	glDeleteBuffers(1, &m_RendererID);
}

void ColorBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void ColorBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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