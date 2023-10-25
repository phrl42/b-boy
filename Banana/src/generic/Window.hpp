#pragma once
#include "event/Event.h"


namespace Banana
{
  struct WindowProps
  {
    std::string Title;
    unsigned int width;
    unsigned int height;
  
    WindowProps(const std::string& title = "Banana Engine", unsigned int width = 1280, unsigned int height = 720)
      : Title(title), width(width), height(height)
      {

      }
  };

  class Window
  {
  public:
    using EventCallbackFunction = std::function<void(Event&)>;
    enum FRAMEWORK
    {
      NONE = 0,
      GLFW
    };
    virtual ~Window() = default;

    virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;

    virtual unsigned int GetWidth() = 0;
    virtual unsigned int GetHeight() = 0;
    
    virtual double GetTime() = 0;

    virtual void Resize(unsigned int width, unsigned int height) = 0;
    virtual void SetEventCallback(const EventCallbackFunction& callback_function) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;
    virtual void EnableFullscreen(bool enabled) = 0;
    virtual void* GetNativeWindow() const = 0;

    static FRAMEWORK GetFramework() { return framework; }
    static std::unique_ptr<Window> Create(const WindowProps& windowProps = WindowProps());

  private:
    static FRAMEWORK framework;  
  };

};
