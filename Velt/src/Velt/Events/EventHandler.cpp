#include "EventHandler.h"
#include "Event.h"
#include "ImGui/ImGuiLayer.h"
#include "Platform/Windows/WindowsInput.h"

namespace Velt
{
    void EventHandler::ProcessEvent(Event& event)
    {
        while (SDL_PollEvent(event.Data()))
        {
            Input::ProcessEvent(event.Ref());
            ImGuiLayer::ProcessSDLEvent(event.Data());

            switch (event->type)
            {
                case SDL_EVENT_QUIT:
                {

                }
            }
        }
    }
}



/*
* 	while (SDL_PollEvent(&event))
{

VT_PROFILE_SCOPE("SDL PollEvent Loop");

// TODO: Dont pass raw sdl events

Input::ProcessEvent(event);
ImGuiLayer::ProcessSDLEvent(&event);

if (Input::IsKeyDown(Scancode::VELT_SCANCODE_N))
{
running = false;
}

switch (event.type)
{
case SDL_EVENT_QUIT:
{
VT_PROFILE_SCOPE("WindowClose Event");
WindowCloseEvent e;
OnEvent(e);
running = false;
break;
}
break;
case SDL_EVENT_WINDOW_RESIZED:
case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
{
VT_PROFILE_SCOPE("WindowResize Event");
u16 w = (u16)event.window.data1;
u16 h = (u16)event.window.data2;
WindowResizeEvent e(w, h);
OnEvent(e);
}
break;
default:
break;
}
} */