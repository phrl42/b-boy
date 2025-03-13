#pragma once

#include "event/Event.h"

namespace Banana
{
	
  class WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() = default;
    ~WindowCloseEvent() override = default;
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent final : public Event
	{
	private:
		unsigned int width, height;
	public:
		WindowResizeEvent() = default;
    ~WindowResizeEvent() override = default;
		WindowResizeEvent(const unsigned int width, const unsigned int height)
			: width(width), height(height) { }

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }
		
    std::string ToString() const override
		{
			std::string ss = "";
			ss = "WindowResizeEvent width: " + std::to_string(width) + " height: " + std::to_string(height);
      return ss;
    }
		
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)
	};
	
  class AppTickEvent final : public Event
	{
	public:
		AppTickEvent() = default;
    ~AppTickEvent() override = default;
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
  class AppUpdateEvent final : public Event
	{
	public:
		AppUpdateEvent() = default;
    ~AppUpdateEvent() override = default;
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
  class AppRenderEvent final : public Event
	{
	public:
		AppRenderEvent() = default;
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
};

