#pragma once
#include "Log.h"


extern Lavendel::Application* Lavendel::CreateApplication();

int main(int argc, char** argv)
{
	
	Lavendel::Log::Init();

	LV_CORE_INFO("Core Log Initialized!");
	LV_INFO("Client Log Initialized!");


	auto app = Lavendel::CreateApplication();
	app->Run();
	delete app;
}



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
#endif 
