#pragma once
#include "Core/Core.h"
#include "Core/Math.h"

namespace Velt 
{
	struct alignas(16) Light 
	{
		Vector4 Color = Vector4(1.0f);
		Vector Position = Vector(1.0f);
		float Intensity = 0.0f;
	};

	struct LightUBO
	{
		Light Lights[MAX_LIGHTS];
		int _pad01[3];
		int Count;
	};
}