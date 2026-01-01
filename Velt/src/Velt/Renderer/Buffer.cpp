#include "vtpch.h"
#include "Buffer.h"

namespace Velt::Renderer 
{
	void BufferLayout::CalculateOffsetAndStride()
	{
		u32 offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

}