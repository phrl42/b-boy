#include "incs.h"
#include "renderer/Renderer.h"
#include "renderer/Renderer2D.h"

#include "renderer/RenderCommand.hpp"

namespace Banana
{

  void Renderer::OnWindowResize(uint32_t width, uint32_t height)
  {
    RenderCommand::SetViewPort(0, 0, width, height);
  }

  void Renderer::Init()
  {
    RenderCommand::Init();
    Renderer2D::Init();
  }

  void Renderer::Shutdown()
  {
    Renderer2D::Shutdown();
  }

  void Renderer::BeginScene()
  {

  }

  void Renderer::EndScene()
  {

  }

  void Renderer::Submit(const Shr<VertexArray>& vertex_array)
  {
    RenderCommand::DrawIndexed(vertex_array);
  }

};