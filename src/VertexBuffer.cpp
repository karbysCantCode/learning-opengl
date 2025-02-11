#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum GLmode)// size is size of data in bytes
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLmode));
    m_BufferSize = size;
}

//VertexBuffer::VertexBuffer(const void* data, unsigned int size)
//{
//    GLCall(glGenBuffers(1, &m_RendererID));
//    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
//    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
//}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_RendererID))
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::UpdateSection(unsigned int offset, const void* data, unsigned int size, GLenum GLmode) // size is size of data in bytes
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    if (size >= m_BufferSize)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GLmode)); // NULL PTR ERASES ALL DATA< MAKE SO DOESNT BUT STILLE XPALNDS AND ALSO DO THAT IN IDNEX BUFFER
        m_BufferSize += size;
    }
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::RewriteBuffer(const void* data, unsigned int size, GLenum GLmode) // size is size of data in bytes
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLmode));
    m_BufferSize = size;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
