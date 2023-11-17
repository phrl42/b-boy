#pragma once
#include "incs.h"
#include "ecs/Component.h"
#include "renderer/Texture.h"

namespace Banana
{
  class QuadComponent : public Component
  {
  public:
    virtual ~QuadComponent() = default;
    QuadComponent();
    QuadComponent(const std::string &path);
    QuadComponent(const TextureSpecification &spec);

    virtual void OnUpdate(float dt, const Transform &transform) override;

    void UpdateTexture(const TextureSpecification &spec);
    inline uint32_t GetTextureID()
    {
      return tex->GetRendererID();
    }
  private:
  Shr<Texture2D> tex = nullptr;
  };
};
