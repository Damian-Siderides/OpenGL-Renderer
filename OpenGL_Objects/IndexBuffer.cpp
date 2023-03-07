#include "IndexBuffer.h"

#include <Renderer.h>

IndexBuffer::IndexBuffer() {
	
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {

	// ASSERT (sizeof(unsigned int()) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


// class IndexBuffer {
// private:
// 	unsigned int m_RendererID;

// public:
// 	GLuint IndexBuffer;

// 	IndexBuffer(const void* data, unsigned int size);

// 	void Bind();
// 	void Unbind();

// 	IndexBuffer() {
		
// 	}



// }



// class IndexBuffer {
// public:

// 	IndexBuffer() {
// 		GLuint ibo;
// 		glGenBuffers(1, &ibo);
// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
// 		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_STATIC_DRAW);

// 	}
// }