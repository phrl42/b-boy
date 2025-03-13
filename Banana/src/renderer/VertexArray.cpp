#include "incs.h"

#include "platform/OpenGL/OpenGLVertexArray.h"
#include "renderer/Renderer.h"

namespace Banana
{
  Shr<VertexArray> VertexArray::Create()
  {
    switch(Renderer::GetAPI())
    {
      case RendererAPI::API::OpenGL: return MakeShr<OpenGLVertexArray>();
    }

    LOG("Could not return a vertex array! What the hell did you do??");
    return nullptr;
  }
};