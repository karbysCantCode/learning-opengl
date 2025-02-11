#pragma once
#include "GL/glew.h"

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_BufferSize = 0;

public:
	VertexBuffer(const void* data, unsigned int size, GLenum GLmode = GL_STATIC_DRAW);
	//VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateSection(unsigned int offset, const void* data, unsigned int size, GLenum GLmode);

	void RewriteBuffer(const void* data, unsigned int size, GLenum GLmode);

};