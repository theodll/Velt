//
// Created by Theo Wimber on 15.11.25.
//

#include "vtpch.h"
#include "Layer.h"


namespace Velt
{
    Layer::Layer(const std::string& rDebugName) : m_DebugName(rDebugName)
    {
        VT_PROFILE_FUNCTION();
    }

    Layer::~Layer()
    {
        VT_PROFILE_FUNCTION();
    }
}