#pragma once

#include "ecs/components/QuadComponent.h"
#include "gbc/ppu.h"

namespace Banana
{
  class TileComponent : public Component
  {
  public:
    TileComponent(GBC::Tile *tile);

    void UpdateTile(GBC::Tile *tile);
    void UpdateTileData();

    virtual ~TileComponent() = default;
    virtual void OnUpdate(float dt, const Transform &transform) override;

    uint32_t GetTextureID();

  private:
    struct Pixel
    {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      //uint8_t a;
    };

    Pixel pixels[8*8];
    QuadComponent quad;
    GBC::Tile *tile;

    TextureSpecification spec;
  };
};
