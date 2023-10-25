#pragma once

#include "renderer/RendererAPI.h"

namespace Banana
{
  class Renderer 
  {
  public:
    static void OnWindowResize(uint32_t width, uint32_t height);
    static void Init();
    static void Shutdown();
    
    static void BeginScene();
    static void EndScene();

    static void Submit(const Shr<VertexArray>& vertex_array);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  private:
  };
};