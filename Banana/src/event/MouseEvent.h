#pragma once


namespace Banana
{

  class MouseButtonPressedEvent final : public Event
  {
  private:
    int button;
    int mods;

  public:
    MouseButtonPressedEvent(int button, int mods)
    : button(button), mods(mods)
    {

    }

    inline int GetButton() const { return button; }
    inline bool isModPressed(const int mod) const { return mods & mod; }


    std::string ToString() const override
    {
      std::string ss = "";
      ss = "MouseButtonPressedEvent: " + std::to_string(button) + "\n"; 
      return ss;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput);

    EVENT_CLASS_TYPE(MouseButtonPressed);
  };

  class MouseButtonReleasedEvent final : public Event
  {
  private:
    int button;
    int mods;

  public:
    MouseButtonReleasedEvent(int button, int mods)
    : button(button), mods(mods)
    {

    }

    inline int GetButton() const { return button; }
    inline bool isModPressed(const int mod) const { return mods & mod; }


    std::string ToString() const override
    {
      std::string ss = "";
      ss = "MouseButtonReleasedEvent: " + std::to_string(button) + "\n"; 
      return ss;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput);

    EVENT_CLASS_TYPE(MouseButtonReleased);
  };

  class MouseScrolledEvent final : public Event
  {
  private:
    float xOff;
    float yOff;

  public:
    MouseScrolledEvent() = default;
    MouseScrolledEvent(float xOff, float yOff)
    : xOff(xOff), yOff(yOff)
    {

    }

    inline float GetXOffset() const { return xOff; }
    inline float GetYOffset() const { return yOff; }

    std::string ToString() const override
    {
      std::string ss = "";
      ss = "MouseScrolledEvent X-Offset: " + std::to_string(xOff) + "Y-Offset: " + std::to_string(yOff) + "\n";
      return ss; 
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

    EVENT_CLASS_TYPE(MouseScrolled);
  };

  class MouseMovedEvent final : public Event
  {
  private:
    float xOff;
    float yOff;

  public:
    MouseMovedEvent() = default;
    MouseMovedEvent(float xOff, float yOff)
    : xOff(xOff), yOff(yOff)
    {

    }

    inline float GetXOffset() const { return xOff; }
    inline float GetYOffset() const { return yOff; }

    std::string ToString() const override
    {
      std::string ss = "";
      ss = "MouseScrolledEvent X-Offset: " + std::to_string(xOff) + "Y-Offset: " + std::to_string(yOff) + "\n";
      return ss; 
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

    EVENT_CLASS_TYPE(MouseMoved);
  };

};