#pragma once
#include "renderer/Texture.h"

#include "generic/Camera.h"
#include "generic/Font.h"
#include "Projection.h"

namespace Banana
{
  struct Texture2D;


  class Renderer2D
  {
  public:

    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& cam);    
    static void BeginScene();    
    static void EndScene();

    static void StartBatch();
    static void NextBatch();

    static void Flush();

    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rotation, Projection proj = NONE);
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, Projection proj = NONE);
    
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, const Shr<Texture2D>& texture, Projection proj = NONE);
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rotation, const Shr<Texture2D>& texture, Projection proj = NONE);

    static void DrawText(const std::string& text, Shr<Font> font, const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color, Projection proj = NONE);
  };
};
