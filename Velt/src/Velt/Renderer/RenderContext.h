#pragma once
#include "Velt/Core/Core.h"

namespace Velt
{
	class Context
	{
	public:
		Context() = default;
		virtual ~Context() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		static std::unique_ptr<Context> Create();
	};
}

