#pragma once
#include "Core/Core.h"
#include "ShaderLibrary.h"
#include "Buffer.h"
#include "Pipeline.h"

namespace Velt 
{


	class PipelineManager
	{
	public:
		PipelineManager() = default;
		virtual ~PipelineManager() {};
		void Init();
		void Shutdown();

		static VkPipelineCache GetVulkanCache() { return m_PipelineCache; }
		static void SaveCache();
	private:
		static VkPipelineCache m_PipelineCache;
		static std::filesystem::path m_CachedFilePath;
	};
}