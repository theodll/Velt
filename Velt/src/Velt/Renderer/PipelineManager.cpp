#include "PipelineManager.h"

namespace Velt 
{
	
	VkPipelineCache PipelineManager::m_PipelineCache;
	std::filesystem::path PipelineManager::m_CacheFilePath = "Assets/Cache/pipeline_cache.cache";


	void PipelineManager::Init()
	{
		VT_PROFILE_FUNCTION();
		std::vector<std::byte> cacheData;
		if(std::filesystem::exists(m_CacheFilePath))
		{
			std::ifstream file(m_CacheFilePath, std::ios::binary | std::ios::ate);
			size_t size = file.tellg();
			cacheData.resize(size);
			file.seekg(0);
			file.read((char*)cacheData.data(), size);
			VT_CORE_INFO("Pipeline Cache ({0} Bytes) successfully loaded", size);
		}

		VkPipelineCacheCreateInfo cacheInfo{};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		if (!cacheData.empty()) 
		{
			cacheInfo.initialDataSize = cacheData.size();
			cacheInfo.pInitialData = cacheData.data();
		}
		VT_VK_CHECK(vkCreatePipelineCache(RHI::VulkanContext::GetDevice()->device(), &cacheInfo, VT_NULL_HANDLE, &m_PipelineCache), "Failed to create Pipeline Cache");
	}

	void PipelineManager::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		SaveCache();
		vkDestroyPipelineCache(RHI::VulkanContext::GetDevice()->device(), m_PipelineCache, VT_NULL_HANDLE);
		m_PipelineCache = VT_NULL_HANDLE;
	}

	void PipelineManager::SaveCache()
	{
		VT_PROFILE_FUNCTION();
		if (!m_PipelineCache) return;

		size_t cacheSize = 0;
		vkGetPipelineCacheData(RHI::VulkanContext::GetDevice()->device(), m_PipelineCache, &cacheSize, VT_NULL_HANDLE);

		std::vector<std::byte> cacheData(cacheSize);
		vkGetPipelineCacheData(RHI::VulkanContext::GetDevice()->device(), m_PipelineCache, &cacheSize, cacheData.data());

		std::filesystem::create_directories("Assets/Cache");
		std::ofstream file(m_CacheFilePath, std::ios::binary);
		file.write((char*)cacheData.data(), cacheSize);
		VT_CORE_INFO("Pipeline Cache ({0} Bytes) successfully written", cacheSize);
	}
}