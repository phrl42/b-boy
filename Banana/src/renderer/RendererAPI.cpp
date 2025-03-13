#include "incs.h"
#include "renderer/RendererAPI.h"

namespace Banana
{
  #ifdef MACRO_OPENGL
  RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;
  #endif
};