#pragma once
#include "Core/Assert.h"
#include "Core/Core.h"

namespace Velt
{

	enum class ShaderDataType : uint8_t
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static u32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3; // 9 Floats
		case ShaderDataType::Mat4:     return 4 * 4 * 4; // 16 Floats
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 4; 
		case ShaderDataType::None:     return 0;
		}
		VT_CORE_ASSERT(false, "Unknown ShaderDataType");
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
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0) {};

		u32 getComponentCount() const 
		{
			switch(Type) 
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;	
				case ShaderDataType::None:		return 0;	
			}

			return 0;
		}
	};

	class VELT_API BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<BufferElement> element)
			: m_Elements(element)
		{
			CalculateOffsetAndStride();	
		};

		BufferLayout()
		{
			CalculateOffsetAndStride(); 
		};

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		u32 GetElementCount() const { return (u32)m_Elements.size(); }
		u64 getStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() {  return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() {	return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride();
		std::vector<BufferElement> m_Elements;
		u64 m_Stride;
	};
}