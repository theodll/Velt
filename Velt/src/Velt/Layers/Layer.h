#pragma once
#include "vtpch.h"
#include "Events/Event.h"

namespace Velt
{
    class Velt_API Layer {
    public:
        Layer(const std::string& debugName = "Layer" );
        virtual ~Layer();

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate() {};
        virtual void OnEvent(Event& event) {};
        virtual void OnImGuiRender() {};
        
        // Optional: Called during rendering phase, useful for layers that need to render
        // graphics directly to the command buffer. This is called after scene rendering
        // and respects the layer stack order, so layers added later will render on top.
        virtual void OnRender(void* commandBuffer) {};

        inline const std::string& GetName() const {  return m_DebugName;   }

    protected:
        std::string m_DebugName;
    };
}