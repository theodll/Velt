#pragma once
#include "Core/Core.h"

namespace Velt::Renderer
{

	enum class ShaderDataType : u8
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static u32 ShaderDataTypeSize(ShaderDataType type) 
	{
		switch (type) 
		{
			case ShaderDataType::Float:		return sizeof(float);
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		VT_CORE_ASSERT(false, "Unknown ShaderDataType")
		return 0;

	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		u32 Size;
		u32 Count;
		u32 Offset;

		BufferElement(ShaderDataType type, const std::string& name)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0)
	};

	class VELT_API BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<BufferElement>& element)
			: m_Elements(element)
		{
			void calculateOffsetAndStride();
			
		};

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	private:

		void calculateOffsetAndStride() 
		{
			u32 offset;
			m_Stride = 0; 
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size; 
			}
		};

		std::vector<BufferElement> m_Elements;
		u64 m_Stride;
	};
}