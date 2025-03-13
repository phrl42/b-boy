#pragma once
#include "renderer/RendererAPI.h"

namespace Banana
{
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    OpenGLRendererAPI() = default;
    ~OpenGLRendererAPI() override = default;

    virtual void Clear() override;
    virtual void SetClearColor(const glm::vec4& color) override;

    virtual void DrawIndexed(const Shr<VertexArray>& vertex_array, uint32_t index_count = 0) override;

    virtual void Init() override;

    virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    virtual void CopyFramebuffer(uint32_t readID, uint32_t drawID, unsigned int width, unsigned int height) override;
  };
};
