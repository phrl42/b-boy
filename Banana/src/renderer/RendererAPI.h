#pragma once


#include "renderer/VertexArray.h"
#include "glm.hpp"

namespace Banana
{
  class RendererAPI 
  {
  public:
    enum class API
    {
      NONE = 0, OpenGL
    };
    virtual ~RendererAPI() = default;

    virtual void Clear() = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;

    virtual void DrawIndexed(const Shr<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;

    virtual void Init() = 0;

    virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    inline static API GetAPI() { return api; }

    virtual void CopyFramebuffer(uint32_t readID, uint32_t drawID, unsigned int width, unsigned int height) = 0;


  private:
    static API api;    
  };
};