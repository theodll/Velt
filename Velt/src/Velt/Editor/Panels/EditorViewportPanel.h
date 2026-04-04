#pragma once
#include "Core/Core.h"
#include "Editor/EditorPanel.h"
#include "Renderer/Renderer.h"
#include <imgui.h>

namespace Velt::Editor 
{
	class VELT_API EditorViewportPanel : public EditorPanel
	{
	public:
		EditorViewportPanel() = default;
		virtual ~EditorViewportPanel() = default;

		void Init(u32 width, u32 height);
		void Shutdown() override;

		void OnUpdate(Timestep ts) override;

		void OnImGuiRender() override;
		void OnImGuiRender2() override;

		void Resize(u32 width, u32 height);

		void SetRenderTarget(RenderTarget target);
		RenderTarget GetRenderTarget() { return m_RenderTarget; }

		u32 GetWidth() const { return m_Width; }
		u32 GetHeight() const { return m_Height; }

		void ProcessPendingChanges();
		bool RenderTargetChangePending;
		RenderTarget PendingRenderTarget;

	private:
		void CreateResources();
		void CreateDescriptorSet();

		RenderTarget m_RenderTarget = VT_RENDER_TARGET_COMPOSITE;

		u32 m_Width{};
		u32 m_Height{};

		u32 m_PendingWidth{};
		u32 m_PendingHeight{};
		bool m_ResizePending = false;

		ImVec2 m_LocalMousePos; 

		VkImage m_Image = VT_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VT_NULL_HANDLE;
		VkImageView m_ImageView = VT_NULL_HANDLE;
		VkSampler m_Sampler = VT_NULL_HANDLE;

		VkDescriptorSet m_DescriptorSet = VT_NULL_HANDLE;

	friend class EditorLayer;
	friend class EditorGuizmos;
	};
}