#include "VertexArray.h"

#include <VertexBufferLayout.h>
#include <Renderer.h>

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {

	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		// glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);

		// GLCall(glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0));
		GLCall(glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, element.count * sizeof(float), (void*) 0));

		// offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}