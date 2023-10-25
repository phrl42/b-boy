#include "incs.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"
#include "renderer/Renderer.h"

namespace Banana
{
  Shr<Framebuffer> Framebuffer::Create(const FramebufferProperties& fb)
  {
    switch(Renderer::GetAPI())
    {
      case RendererAPI::API::OpenGL: return MakeShr<OpenGLFramebuffer>(fb);
    }
    
    LOG("RendererAPI in Framebuffer not found.");
    return nullptr;
  }
};