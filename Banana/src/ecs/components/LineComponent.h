#pragma once

#include "ecs/components/QuadComponent.h"
#include "gbc/ppu.h"

namespace Banana
{
  class LineComponent : public Component
  {
  public:
    LineComponent(GBC::Screen *screen);
    
    void UpdateTileData();

    virtual ~LineComponent() = default;
    virtual void OnUpdate(float dt, const Transform &transform) override;

    uint32_t GetTextureID();

  private:
    struct Pixel
    {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a;
    };

    std::vector<Pixel> pixels;

    QuadComponent quad;

    GBC::Screen *screen;
    uint32_t n;
    
    TextureSpecification spec;
  };
};
