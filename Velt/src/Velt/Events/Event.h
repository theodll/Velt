#pragma once
#include <cstdint>
#include <string>

namespace Velt {

	enum class EventType : uint8_t {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled, GassyGoonToonX
	};

	enum EventCategory : uint32_t {
		EventCategoryNone        = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput       = 1 << 1,
		EventCategoryKeyboard    = 1 << 2,
		EventCategoryMouse       = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

	class Event {
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual uint32_t GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory cat) const { return (GetCategoryFlags() & cat) != 0; }

		bool IsHandled = false;
	};

} // namespace Velt

#define VELT_EVENT_CLASS_TYPE(type) \
static Velt::EventType GetStaticType() { return Velt::EventType::type; } \
Velt::EventType GetEventType() const override { return GetStaticType(); } \
const char* GetName() const override { return #type; }

#define VELT_EVENT_CLASS_CATEGORY(flags) \
uint32_t GetCategoryFlags() const override { return flags; }
