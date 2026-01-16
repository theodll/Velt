#pragma once
#include "Velt/Core/Core.h"
#include <string>
#include "Buffer.h"

namespace Velt::Renderer
{
	struct PipelineSpecification
	{
		BufferLayout Layout;	
		std::string VertexShaderPath; // this has to be the compiled binary right now, not the Source ! 
		std::string FragmentShaderPath; // this too
		std::string DebugName;
	};



	class VELT_API Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification& GetSpecification() = 0;
		virtual const PipelineSpecification& GetSpecification() const = 0;

		virtual void Init() = 0;
		virtual void Invalidate() = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};

}