#pragma once
#include "Core/Input.h"
#include <SDL3/SDL.h>


namespace Velt::SDL
{
	class Input : public Velt::Input
	{
	protected:
		virtual void ProcessEventImpl(const SDL_Event& e) override;
		virtual void BeginFrameImpl() override;

		virtual bool IsKeyDownImpl(Scancode code) override;
		virtual bool IsKeyPressedImpl(Scancode code) override;
		virtual bool IsKeyReleasedImpl(Scancode code) override;

		void HandleKeyDown(const SDL_Event& e);
		void HandleKeyUp(const SDL_Event& e);
		
	private:
		std::array<bool, SDL_SCANCODE_COUNT> s_Down{};
		std::array<bool, SDL_SCANCODE_COUNT> s_Pressed{};
		std::array<bool, SDL_SCANCODE_COUNT> s_Released{};
	};
}