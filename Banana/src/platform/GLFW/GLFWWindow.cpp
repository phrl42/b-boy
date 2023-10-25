#include "incs.h"
#include "GLFWWindow.h"

#include "event/Event.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"
#include "event/ApplicationEvent.h"

#include "glad.h"
#include "GLFW/glfw3.h"

namespace Banana
{
  static bool glfw_init = false;
  static void ErrorCallback(int error, const char* msg)
  {
    LOG("GLFW ERROR " + std::to_string(error) + ":" + msg);
  }

  GLFWWindow::~GLFWWindow()
  {
    Quit();
  }
  
  GLFWWindow::GLFWWindow(const WindowProps& window_props)
  {
    Init(window_props);
  }

  void GLFWWindow::SwapBuffers()
  {
    context->SwapBuffer();
  }

  void GLFWWindow::Resize(unsigned int width, unsigned int height)
  {

  }

  void* GLFWWindow::GetNativeWindow() const
  {
    return (void*)glfwWindow;
  }

  void GLFWWindow::EnableFullscreen(bool enabled)
  {

  }


  void GLFWWindow::Init(const WindowProps& window_props)
  {
    windowData.title = window_props.Title;
    windowData.height = window_props.height;
    windowData.width = window_props.width;

    if(!glfw_init)
    {
      if(!glfwInit())
      {
        LOG("Could not initialize glfw");
        exit(-1);
      }
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindow = glfwCreateWindow((int)window_props.width, 
      (int)window_props.height, windowData.title.c_str(), nullptr, nullptr);

    // context
    context = Context::CreateContext((void*)glfwWindow);
    context->Init();

    glfwSetWindowUserPointer(glfwWindow, &windowData);

    SetVSync(true);

    // set glfw callbacks

    glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height)
    {
      WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
      data.width = width;
      data.height = height;
      
      WindowResizeEvent event(width, height);

      data.callback(event);
    });

    glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* window)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        
        WindowCloseEvent event = WindowCloseEvent();

        data.callback(event);
    });

    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch(action)
      {
        case GLFW_PRESS:
        {
          KeyPressedEvent event(key, 0, mods);
          data.callback(event);
          break;
        }
        case GLFW_RELEASE:
        {
          KeyPressedEvent event(key, 0, mods);
          data.callback(event);
          break;
        }

        case GLFW_REPEAT:
        {
          KeyPressedEvent event(key, 1, mods);
          data.callback(event);
          break;
        }

        default:
          break;
      }
    });

    glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch(action)
      {
        case GLFW_PRESS:
        {
          MouseButtonPressedEvent event(button, mods);
          data.callback(event);
          break;
        }

        case GLFW_RELEASE:
        {
          MouseButtonReleasedEvent event(button, mods);
          data.callback(event);
          break;
        }
      }

    });

    glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data.callback(event);
    });

    glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double x, double y)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      MouseMovedEvent event((float)x, (float)y);
      data.callback(event);
    });

  }

  unsigned int GLFWWindow::GetHeight()
  {
    return windowData.height;
  }

  unsigned int GLFWWindow::GetWidth()
  {
    return windowData.width;
  }

  double GLFWWindow::GetTime()
  {
    return glfwGetTime();
  }

  void GLFWWindow::PollEvents()
  {
    glfwPollEvents();
  }

  bool GLFWWindow::IsVSync() const
  {
    return windowData.vsync;
  }

  void GLFWWindow::SetVSync(bool set)
  {
    glfwSwapInterval(set);
    windowData.vsync = set;
  }

  void GLFWWindow::Quit() const
  {
    context->Shutdown();
    delete context;

    glfwDestroyWindow(this->glfwWindow);
  }

};