#include "incs.h"

#include "renderer/RenderCommand.hpp"
#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Banana
{
#ifdef MACRO_OPENGL
  RendererAPI* RenderCommand::api = new OpenGLRendererAPI();
#endif
};