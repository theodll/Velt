//
// Created by Theo Wimber on 15.11.25.
//

#include "lvpch.h"
#include "Layer.h"


namespace Lavendel
{
    Layer::Layer(const std::string& debugName) : m_DebugName(debugName)
    {
        LV_PROFILE_FUNCTION();
    }

    Layer::~Layer()
    {
        LV_PROFILE_FUNCTION();
    }
}