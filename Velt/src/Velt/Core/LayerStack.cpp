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
        for (Layer* layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* pLayer)
    {
        VT_PROFILE_FUNCTION();
        m_LayerInsertIndex = m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, pLayer) - m_Layers.begin();
        ++m_LayerInsertIndex;
        pLayer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* pOverlay)
    {
        VT_PROFILE_FUNCTION();
        m_Layers.emplace_back(pOverlay);
        pOverlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* pLayer)
    {
        VT_PROFILE_FUNCTION();
        auto it = std::find(m_Layers.begin(), m_Layers.end(), pLayer);
        if (it != m_Layers.end()) {
            const auto index = it - m_Layers.begin();
            pLayer->OnDetach();
            m_Layers.erase(it);
            if (m_LayerInsertIndex > 0 && index < m_LayerInsertIndex) {
                --m_LayerInsertIndex;
            }
        }
    }

	void LayerStack::PopAllLayers()
	{
		for (auto* layer : m_Layers)
		{
			layer->OnDetach();
		}

		m_Layers.clear();
		m_LayerInsertIndex = 0;
	}

    void LayerStack::PopOverlay(Layer* pOverlay)
    {
        VT_PROFILE_FUNCTION();
        auto it = std::find(m_Layers.begin(), m_Layers.end(), pOverlay);
        if (it != m_Layers.end()) {
            pOverlay->OnDetach();
            m_Layers.erase(it);
        }
    }
}