//
// Created by Theo Wimber on 15.11.25.
//

#include "vtpch.h"
#include "LayerStack.h"

namespace Velt
{
    LayerStack::LayerStack() : m_LayerInsertIndex(0)
    {
        VT_PROFILE_FUNCTION();
    }

    LayerStack::~LayerStack()
    {
        VT_PROFILE_FUNCTION();
        for (Layer *layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        VT_PROFILE_FUNCTION();
        m_LayerInsertIndex = m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer) - m_Layers.begin();
        ++m_LayerInsertIndex;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        VT_PROFILE_FUNCTION();
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        VT_PROFILE_FUNCTION();
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            const auto index = it - m_Layers.begin();
            layer->OnDetach();
            m_Layers.erase(it);
            if (m_LayerInsertIndex > 0 && index < m_LayerInsertIndex)
            {
                --m_LayerInsertIndex;
            }
        }
    }

    void LayerStack::PopOverlay(Layer *overlay)
    {
        VT_PROFILE_FUNCTION();
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
}