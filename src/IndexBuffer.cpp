#include "IndexBuffer.h"

#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum GLmode)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GLmode));
    m_BufferSize = count * sizeof(unsigned int);
}

//IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
//    : m_Count(count)
//{
//    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
//
//    GLCall(glGenBuffers(1, &m_RendererID));
//    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
//    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
//}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID))
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::RewriteBuffer(const unsigned int* data, unsigned int count, GLenum GLmode)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    m_Count = count;

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GLmode));
    m_BufferSize = count * sizeof(unsigned int);
}

void IndexBuffer::UpdateSection(unsigned int offset, const void* data, unsigned int count, GLenum GLmode) // DONT USE THIS POS (pience of fshit)
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    /*if (count * sizeof(unsigned int) >= m_BufferSize)
    {
        m_BufferSize += count * sizeof(unsigned int);
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, nullptr, GLmode));
    }*/
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(unsigned int), data));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
