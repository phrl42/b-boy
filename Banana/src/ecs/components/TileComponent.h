#pragma once

#include "ecs/components/QuadComponent.h"
#include "gbc/ppu.h"

namespace Banana
{
  class TileComponent : public Component
  {
  public:
    TileComponent(GBC::Tile (&tile)[], uint32_t n, uint32_t width, uint32_t height);
    
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

    Pixel pixels[166 * 144] = {0};

    QuadComponent quad;

    //std::vector<GBC::Tile*> tiles;

    GBC::Tile (&tile)[];
    uint32_t n;
    
    TextureSpecification spec;
  };
};
