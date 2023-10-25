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

    virtual void OnUpdate(float dt, const Transform &transform) override;
  private:
  Shr<Texture2D> tex = nullptr;
  };
};