#pragma once
#include "lvpch.h"
#include "Log.h"


extern Lavendel::Application* Lavendel::CreateApplication();


int main(int argc, char** argv)
{
	

	Lavendel::Log::Init();

	LV_CORE_INFO("Core Log Initialized!");
	LV_INFO("Client Log Initialized!");

	LV_PROFILE_BEGIN_SESSION("Startup", "LavendelProfile-Startup.json");
	auto app = Lavendel::CreateApplication();
	LV_PROFILE_END_SESSION();
	LV_PROFILE_BEGIN_SESSION("Runtime", "LavendelProfile-Runtime.json");
	app->Run();
	LV_PROFILE_END_SESSION();
	LV_PROFILE_BEGIN_SESSION("Shutdown", "LavendelProfile-Shutdown.json");
	delete app;
	LV_PROFILE_END_SESSION();
}

/*

#ifdef LV_PLATFORM_LINUX

extern Lavendel::Application* Lavendel::CreateApplication();

int main(int argc, char** argv)
{
	Lavendel::Log::Init();
	auto app = Lavendel::CreateApplication();
	app->Run();
	delete app;
}

#endif 


#ifdef LV_PLATFORM_OSX
extern Lavendel::Application* Lavendel::CreateApplication();

int main(int argc, char** argv)
{
	Lavendel::Log::Init();
	auto app = Lavendel::CreateApplication();
	app->Run();
	delete app;
}
#endif */
