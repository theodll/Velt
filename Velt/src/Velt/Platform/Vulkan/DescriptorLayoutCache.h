#include "Core/Core.h"
#include <unordered_map>

#include "DescriptorSetManager.h"

namespace Velt
{
	typedef VkDescriptorSetLayout DescriptorSetLayoutHandle;
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
        u32 binding;
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

        DescriptorSetLayoutHandle CreateLayout(std::vector<DescriptorBinding> *bindings);

	private:
        VkDescriptorSetLayout CreateVulkanLayout(std::vector<DescriptorBinding> *bindings);
        size_t HashBindings(std::vector<DescriptorBinding>* bindings);

        template<typename T>
        void HashCombine(size_t* seed, const T& v)
        {
            seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        std::unordered_map<size_t, VkDescriptorSetLayout> m_Cache;

	};
}