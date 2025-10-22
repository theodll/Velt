#pragma once
#include "Core.h"

namespace Lavendel
{

	class LAVENDEL_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
	};

	// MUST BE DEFINED IN CLIENT
	Application* CreateApplication();

}
