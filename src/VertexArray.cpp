#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

//debug
#include <iostream>

VertexArray::VertexArray()

{
	GLCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	int size = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	std::cout << "Index Buffer Size: " << size << " bytes" << std::endl;
	const auto &elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
