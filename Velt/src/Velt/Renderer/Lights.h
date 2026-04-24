#pragma once
#include "Core/Core.h"
#include "Core/Math.h"

namespace Velt 
{
	struct Light 
	{
		alignas(16) Vector4 Color;
		float Intensity;
	};

	struct LightData 
	{
		Light Lights[MAX_LIGHTS];
		int Count;
	};
}