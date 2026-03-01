#pragma once
#include <SDL3/SDL.h>
#include <memory>

#include "SDLInput.h"
#include "Velt/Events/ApplicationEvents.h"
#include "Velt/Events/KeyEvents.h"
#include "Velt/Events/MouseEvents.h"
#include "Velt/ImGui/ImGuiLayer.h"

namespace Velt::SDL {

    inline Scope<Event> TranslateSDLEvent(const SDL_Event* pEvent) {
        ImGuiLayer::ProcessSDLEvent(pEvent);
        Input::ProcessEvent(pEvent);

        switch (pEvent->type) {

            case SDL_EVENT_QUIT:
                return CreateScope<WindowCloseEvent>();

            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                return CreateScope<WindowResizeEvent>(
                    (int)pEvent->window.data1, (int)pEvent->window.data2
                );
            case SDL_EVENT_KEY_DOWN: {
                const bool repeat = pEvent->key.repeat != 0;
                return CreateScope<KeyPressedEvent>((int)pEvent->key.key, repeat);
            }

            case SDL_EVENT_KEY_UP:
                return CreateScope<KeyReleasedEvent>((int)pEvent->key.key);

            case SDL_EVENT_MOUSE_MOTION:
                return CreateScope<MouseMovedEvent>((float)pEvent->motion.x, (float)pEvent->motion.y);

            case SDL_EVENT_MOUSE_WHEEL:
                return CreateScope<MouseScrolledEvent>((float)pEvent->wheel.x, (float)pEvent->wheel.y);

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                return CreateScope<MouseButtonPressedEvent>((int)pEvent->button.button);

            case SDL_EVENT_MOUSE_BUTTON_UP:
                return CreateScope<MouseButtonReleasedEvent>((int)pEvent->button.button);

            default:
                return nullptr;
        }
    }
} 
