#include "incs.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

#include "glad.h"
#include "GLFW/glfw3.h"

#include "platform/OpenGL/OpenGLBuffer.h"
#include <cstdint>

namespace Banana
{
  void OpenGLRendererAPI::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRendererAPI::DrawIndexed(const Shr<VertexArray> &vertex_array, uint32_t index_count)
  {
    vertex_array->Bind();

    uint32_t count = index_count ? index_count : vertex_array->GetElementBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
  }

  void OpenGLRendererAPI::Init()
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
  {
    glViewport(x, y, width, height);
  }

  void OpenGLRendererAPI::CopyFramebuffer(uint32_t readID, uint32_t drawID, unsigned int width, unsigned int height)
  {
    glBlitNamedFramebuffer(readID, drawID, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
  }

};