#pragma once
#include "vtpch.h"
#include "Events/Event.h"
#include "Renderer/RenderCommandBuffer.h"
#include "Core/Timestep.h"

namespace Velt
{
    class VELT_API Layer {
    public:
        Layer(const std::string& debugName = "Layer" );
        virtual ~Layer();

        virtual void Init() {};
        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate(Timestep ts) {};
        virtual void OnEvent(Event& event) {};
        virtual void OnImGuiRender() {};
        virtual void OnRender(VkCommandBuffer commandBuffer) {};

        inline const std::string& GetName() const {  return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}