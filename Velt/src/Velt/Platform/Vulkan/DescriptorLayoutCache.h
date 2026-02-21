#include "Core/Core.h"
#include <unordered_map>

#include "DescriptorSetManager.h"

namespace Velt
{
	typedef VkDescriptorSetLayout DSLayoutHandle;
}

namespace Velt::RHI
{
    enum class DescriptorType : u8
	{
        SAMPLER,                    // VK_DESCRIPTOR_TYPE_SAMPLER
        COMBINED_IMAGE_SAMPLER,     // VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        SAMPLED_IMAGE,              // VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
        STORAGE_IMAGE,              // VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
        UNIFORM_TEXEL_BUFFER,       // VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
        STORAGE_TEXEL_BUFFER,       // VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
        UNIFORM_BUFFER,             // VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        STORAGE_BUFFER,             // VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        UNIFORM_BUFFER_DYNAMIC,     // VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
        STORAGE_BUFFER_DYNAMIC,     // VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
        INPUT_ATTACHMENT            // VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
    };

    enum class ShaderStage : u8
    {
	    VERTEX, 
        FRAGMENT
    };

    struct DescriptorBinding
    {
        DescriptorType type;
        u32 bindingl;
        u32 count{ 1 };
        ShaderStage stage;
    };

	class DescriptorLayoutCache
	{
	public:
        DescriptorLayoutCache() {};
        virtual ~DescriptorLayoutCache() {};

        void Init();
        void Shutdown();

        DSLayoutHandle CreateLayout(const std::vector<DescriptorBinding>& bindings);

	private:
        VkDescriptorSetLayout CreateVulkanLayout(const std::vector<DescriptorBinding>& bindings);

        std::unordered_map<size_t, VkDescriptorSetLayout> m_Cache;

	};
}