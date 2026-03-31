#pragma once
#include "vtpch.h"
#include "Layer.h"

namespace Velt
{
    class VELT_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void Init() {};
        void Shutdown() {};

        void PushLayer(Layer* pLayer);
        void PushOverlay(Layer* pOverlay);
        void PopLayer(Layer* pLayer);
        void PopOverlay(Layer* pOverlay);

        void PopAllLayers();

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }


    private:
        std::vector<Layer*> m_Layers;
        std::size_t m_LayerInsertIndex;
    };
}


