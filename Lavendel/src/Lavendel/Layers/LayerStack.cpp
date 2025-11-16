//
// Created by Theo Wimber on 15.11.25.
//

#include "lvpch.h"
#include "LayerStack.h"

namespace Lavendel
{
    LayerStack::LayerStack() : m_LayerInsertIndex(0)
    {
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_LayerInsertIndex = m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer) - m_Layers.begin();
        ++m_LayerInsertIndex;
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end()) {
            const auto index = it - m_Layers.begin();
            m_Layers.erase(it);
            if (m_LayerInsertIndex > 0 && index < m_LayerInsertIndex) {
                --m_LayerInsertIndex;
            }
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            m_Layers.erase(it);
        }
    }
}