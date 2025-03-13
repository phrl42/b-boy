#include "incs.h"
#include "platform/OpenGL/OpenGLContext.h"

#include "glad.h"
#include "GLFW/glfw3.h"

namespace Banana
{
  OpenGLContext::OpenGLContext(void* window)
  : window(window)
  {

  }

  // todo: check for platform stuff

  void OpenGLContext::Init()
  {
    glfwMakeContextCurrent((GLFWwindow*)window);
    // init glad stuff
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      LOG("Failed to initialize glad");
    }

  }

  void OpenGLContext::Shutdown()
  {

  }

  void OpenGLContext::SwapBuffer()
  {
    glfwSwapBuffers((GLFWwindow*)window);
  }

};