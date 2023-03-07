#include "Renderer.h"

#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": Line " << line << std::endl;
		return false;
	}
	return true;
}
void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib /*, const Shader &shader */) const {

	//shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_POINTS, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_LINE_STRIP, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); - good one
	// GLCall(glDrawElements(GL_LINE_LOOP, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_LINE_STRIP_ADJACENCY, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_LINES_ADJACENCY, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_TRIANGLE_STRIP, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_TRIANGLE_FAN, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_TRIANGLE_STRIP_ADJACENCY, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_TRIANGLES_ADJACENCY, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	// GLCall(glDrawElements(GL_PATCHES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));



	// GL_POINTS
	// GL_LINE_STRIP
	// GL_LINE_LOOP
	// GL_LINES
	// GL_LINE_STRIP_ADJACENCY
	// GL_LINES_ADJACENCY
	// GL_TRIANGLE_STRIP
	// GL_TRIANGLE_FAN
	// GL_TRIANGLES
	// GL_TRIANGLE_STRIP_ADJACENCY
	// GL_TRIANGLES_ADJACENCY
	// GL_PATCHES

}