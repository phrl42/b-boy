#include "incs.h"
#include "platform/OpenGL/OpenGLContext.h"

namespace Banana
{
  Context* Context::CreateContext(void* window)
  {
    // todo: ask for platform
    return new OpenGLContext(window);
  }
};