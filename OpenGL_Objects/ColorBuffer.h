#pragma once

#include <GL/glew.h>

class ColorBuffer {
private:
	unsigned int m_RendererID;

public:
	// GLuint vertexbuffer;
	unsigned int colorbuffer;

	ColorBuffer(const void* data, unsigned int size);
	~ColorBuffer();

	void Bind() const;
	void Unbind() const;
};