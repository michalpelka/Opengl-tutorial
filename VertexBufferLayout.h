#pragma once
#include <Vector>
#include "Renderer.h"
#include <GL/GL.h>
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;

	}
};
class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_Stride(0)
	{

	}
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}
	template<> void Push<float>(unsigned int count)
	{
		ASSERT(sizeof(float) == sizeof(GL_FLOAT));
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * sizeof(GL_FLOAT);
	}
	template<> void Push<unsigned int>(unsigned int count)
	{
		ASSERT(sizeof(unsigned int) == sizeof(GL_UNSIGNED_INT));
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * sizeof(GL_UNSIGNED_INT);
	}
	template<> void Push<unsigned char>(unsigned int count)
	{
		ASSERT(sizeof(unsigned char) == sizeof(GL_UNSIGNED_BYTE));
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * sizeof(GL_UNSIGNED_BYTE);
	}
	inline const std::vector<VertexBufferElement>& GetElements() const {return m_Elements;}
	inline unsigned int GetStride() const { return m_Stride; }


private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};

