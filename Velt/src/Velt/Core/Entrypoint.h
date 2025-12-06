#pragma once
#include "vtpch.h"
#include "Log.h"

extern Velt::Application *Velt::CreateApplication();

int main(int argc, char **argv)
{
	Velt::Log::Init();

	VT_CORE_INFO("Core Log Initialized!");
	VT_INFO("Client Log Initialized!");

	VT_PROFILE_BEGIN_SESSION("Startup", "VeltProfile-Startup.json");
	auto app = Velt::CreateApplication();
	VT_PROFILE_END_SESSION();

	VT_PROFILE_BEGIN_SESSION("Runtime", "VeltProfile-Runtime.json");
	app->Run();
	VT_PROFILE_END_SESSION();

	VT_PROFILE_BEGIN_SESSION("Shutdown", "VeltProfile-Shutdown.json");
	delete app;
	VT_PROFILE_END_SESSION();
}

/*

#ifdef VT_PLATFORM_LINUX

extern Velt::Application* Velt::CreateApplication();

int main(int argc, char** argv)
{
	Velt::Log::Init();
	auto app = Velt::CreateApplication();
	app->Run();
	delete app;
}

#endif


#ifdef VT_PLATFORM_OSX
extern Velt::Application* Velt::CreateApplication();

int main(int argc, char** argv)
{
	Velt::Log::Init();
	auto app = Velt::CreateApplication();
	app->Run();
	delete app;
}
#endif */
