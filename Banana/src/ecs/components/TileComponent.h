#pragma once

#include "ecs/components/QuadComponent.h"
#include "gbc/ppu.h"

namespace Banana
{
  class TileComponent : public Component
  {
  public:
    TileComponent();
    TileComponent(GBC::Tile *tile);

    void UpdateTile(GBC::Tile *tile);

    virtual ~TileComponent() = default;
    virtual void OnUpdate(float dt, const Transform &transform) override;

  private:
    QuadComponent tile_tex;
    GBC::Tile *tile;

    TextureSpecification spec;
  };
};
