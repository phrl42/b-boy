#include "incs.h"
#include "platform/GLFW/GLFWWindow.h"

namespace Banana
{
  // do macro stuff here
  Window::FRAMEWORK Window::framework = Window::GLFW;

  std::unique_ptr<Window> Window::Create(const WindowProps& windowProps)
  {
    return std::make_unique<GLFWWindow>(windowProps);
  }
};
