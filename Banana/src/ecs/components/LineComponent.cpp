#include "ecs/components/LineComponent.h"

namespace Banana
{
  LineComponent::LineComponent(GBC::Screen *screen)
    :screen(screen)
  {
    this->name = "LineComponent";

    this->spec.width = WIDTH;
    this->spec.height = HEIGHT;

    this->spec.size = spec.width * spec.height * 4;
    this->spec.format = ImageFormat::RGBA8;

    for(uint32_t i = 0; i < spec.width * spec.height; i++)
    {
      pixels.push_back({0, 0, 0, 255});
    }

    this->spec.data = (void*)pixels.data();
    
    this->quad = QuadComponent(&spec);

    UpdateTileData();
  }

  uint32_t LineComponent::GetTextureID()
  {
    return quad.GetTextureID();
  }
 
  void LineComponent::UpdateTileData()
  {
    Pixel palette[4] = {{224, 248, 208, 255}, {136, 192, 112, 255}, {52, 104, 86, 255}, {8, 24, 32, 255}};

    for(uint8_t y = 0; y < HEIGHT; y++)
    {
      for(uint8_t x = 0; x <= WIDTH; x++)
      {
	pixels[(y * WIDTH) + x] = palette[screen->line[y].bpp[x]];
      }
    }

    spec.data = (void*)pixels.data();

    quad.UpdateTexture(); 
  }

  void LineComponent::OnUpdate(float dt, const Transform &transform)
  {
    UpdateTileData();
    quad.OnUpdate(dt, transform);
  }
};