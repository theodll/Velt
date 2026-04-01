#pragma once
#include "vtpch.h"
#include "Events/Event.h"
#include "Renderer/RenderCommandBuffer.h"
#include "Core/Timestep.h"

namespace Velt
{
    class VELT_API Layer {
    public:
        Layer(const std::string& rDebugName = "Layer" );
        virtual ~Layer();

        virtual void Init() {};
        virtual void Shutdown() {};

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate(Timestep ts) {};
        virtual void OnEvent(Event& rEvent) {};
        virtual void OnImGuiRender() {};
        virtual void OnImGuiRender2() {};
        virtual void OnRender(VkCommandBuffer commandBuffer) {};
        virtual void OnDefferedRender(VkCommandBuffer commandBuffer) {};

        // TEMP
        virtual void OnBeforeFrameBegin() {};

        inline const std::string& GetName() const {  return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}