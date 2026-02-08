#pragma once
#include "Velt/Core/Core.h"

/*
namespace Velt {

struct FramebufferSpecification 
{
  u32 Width, Height;
  u32 Samples = 1;

  bool SwapChainTarget = false;
};

class Framebuffer
{
public:
	virtual ~Framebuffer() = default;

	virtual const FramebufferSpecification& GetSpecification() const = 0;
	virtual void Resize(u32 width, u32 height) = 0;
	virtual int ReadPixel(u32 attachmentIndex, int x, int y) = 0;
	virtual void ClearAttachment(u32 attachmentIndex, int value) = 0;
	virtual void BeginRenderPass(void* commandBuffer) = 0;
	virtual void EndRenderPass(void* commandBuffer) = 0;
	virtual void* GetNativeHandle() = 0;

	static Ref<Framebuffer> Create(const FramebufferSpecification& specs);
};
}
*/