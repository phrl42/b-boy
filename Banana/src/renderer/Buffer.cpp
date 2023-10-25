#include "incs.h"
#include "renderer/Buffer.h"
#include "renderer/Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Banana
{
  Shr<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, uint32_t size)
  {
    switch(Renderer::GetAPI())
    {
    
    case RendererAPI::API::NONE:
    {
      LOG("RendererAPI is NONE!!! what the fuck are you doing??!");
      break;
    }
    
    case RendererAPI::API::OpenGL:
    {
      return MakeShr<OpenGLVertexBuffer>(layout, size);
      break;
    }

    default:
    {
      LOG("Something went wrong while detecting RendererAPI");
      return nullptr;
      break;
    }
    }
  }

  Shr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
  {
    switch(Renderer::GetAPI())
    {

    case RendererAPI::API::NONE:
    {
      LOG("RendererAPI is NONE!!! what the fuck are you doing??!");
      break;
    }

    case RendererAPI::API::OpenGL:
    {
      return MakeShr<OpenGLIndexBuffer>(indices, size);
      break;
    }
    
    default:
    {
      LOG("Something went wrong while detecting RendererAPI");
      return nullptr;
      break;
    }
    }
  }

};