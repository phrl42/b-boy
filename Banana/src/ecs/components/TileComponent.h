#pragma once

#include "ecs/components/QuadComponent.h"
#include "gbc/ppu.h"

namespace Banana
{
  class TileComponent : public Component
  {
  public:
    TileComponent();
    TileComponent(Tile tile);

    void UpdateTile(Tile tile);
    virtual ~TileComponent() = default;
    virtual void OnUpdate(float dt, const Transform &transform) override;

  private:
    QuadComponent pixels[16];
  };
};
