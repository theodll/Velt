#pragma once
#include "Log/Log.h"
#ifdef LM_PLATFORM_WINDOWS

extern Lumen::Application* Lumen::CreateApplication();

int main(int argc, char** argv)
{
	
	Lumen::Log::Init();
	LM_CORE_INFO("Core Log Initialized!");
	LM_INFO("Client Log Initialized!");


	auto app = Lumen::CreateApplication();
	app->Run();
	delete app;
}

#endif


#ifdef LM_PLATFORM_LINUX

extern Lumen::Application* Lumen::CreateApplication();

int main(int argc, char** argv)
{
	Lumen::Log::Init();
	auto app = Lumen::CreateApplication();
	app->Run();
	delete app;
}

#endif 


#ifdef LM_PLATFORM_OSX
extern Lumen::Application* Lumen::CreateApplication();

int main(int argc, char** argv)
{
	Lumen::Log::Init();
	auto app = Lumen::CreateApplication();
	app->Run();
	delete app;
}
#endif 
