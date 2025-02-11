#pragma once
#include "GL/glew.h"

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	unsigned int m_BufferSize = 0;
public:
	IndexBuffer(const unsigned int* data, unsigned int count, GLenum GLmode = GL_STATIC_DRAW);
	//IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void RewriteBuffer(const unsigned int* data, unsigned int count, GLenum GLmode);
	void UpdateSection(unsigned int offset, const void* data, unsigned int count, GLenum GLmode);
	inline unsigned int GetCount() const { return m_Count; }
};
