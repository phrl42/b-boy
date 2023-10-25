#pragma once


namespace Banana
{

  class KeyPressedEvent : public Event
  {
  private:
    bool repeated;
    int keyCode;
    int mods;

  public:
    KeyPressedEvent(const int key_code, const int repeat_code, const int mods)
    : repeated(repeat_code > 0), keyCode(key_code), mods(mods)
    {

    }

    inline int getKeyCode() const { return keyCode; }
    inline bool getRepeated() const { return repeated; }
    inline bool IsModPressed(const int mod) const { return mods & mod; }

    inline std::string ToString() const override
    {
      std::string ss = "";
      ss = "KeyPressedEvent: " + std::to_string(keyCode) + " | Repeated: " += std::to_string(repeated) + "\n";
      return ss;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput);

    EVENT_CLASS_TYPE(KeyPressed);

  };

  class KeyTypedEvent : public Event
  {
  private:
    int keyCode;
  public:
    KeyTypedEvent() = default;
    
    KeyTypedEvent(const int key_code)
    : keyCode(key_code)
    {

    }

    inline int getKeyCode() const { return keyCode; }

    std::string ToString() const override
    {
      std::string ss = "";
      ss = "KeyPressedEvent :" + std::to_string(keyCode) + "\n";
      return ss;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput);

    EVENT_CLASS_TYPE(KeyTyped);
  };
};