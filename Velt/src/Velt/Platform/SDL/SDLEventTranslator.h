#pragma once
#include <SDL3/SDL.h>
#include <memory>

#include "SDLInput.h"
#include "Velt/Events/ApplicationEvents.h"
#include "Velt/Events/KeyEvents.h"
#include "Velt/Events/MouseEvents.h"
#include "Velt/ImGui/ImGuiLayer.h"

namespace Velt::SDL {

    inline std::unique_ptr<Event> TranslateSDLEvent(const SDL_Event& e) {
        ImGuiLayer::ProcessSDLEvent(&e);
        Input::ProcessEvent(e);

        switch (e.type) {

            case SDL_EVENT_QUIT:
                return std::make_unique<WindowCloseEvent>();

            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                return std::make_unique<WindowResizeEvent>(
                    (int)e.window.data1, (int)e.window.data2
                );
            case SDL_EVENT_KEY_DOWN: {
                const bool repeat = e.key.repeat != 0;
                return std::make_unique<KeyPressedEvent>((int)e.key.key, repeat);
            }

            case SDL_EVENT_KEY_UP:
                return std::make_unique<KeyReleasedEvent>((int)e.key.key);

            case SDL_EVENT_MOUSE_MOTION:
                return std::make_unique<MouseMovedEvent>((float)e.motion.x, (float)e.motion.y);

            case SDL_EVENT_MOUSE_WHEEL:
                return std::make_unique<MouseScrolledEvent>((float)e.wheel.x, (float)e.wheel.y);

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                return std::make_unique<MouseButtonPressedEvent>((int)e.button.button);

            case SDL_EVENT_MOUSE_BUTTON_UP:
                return std::make_unique<MouseButtonReleasedEvent>((int)e.button.button);

            default:
                return nullptr;
        }
    }

} // namespace Velt
