#include "ecs/components/TileComponent.h"
#include "incs.h"

namespace Banana
{
  TileComponent::TileComponent(GBC::Tile *tile, uint32_t n)
    :tile(tile)
  {
    this->name = "TileComponent";
    this->n = n;
    
    uint32_t max = n;

    if(n > 8) max = 8;
    this->spec.width = 8 * max;

    max = n;
    if(n % 8 != 0)
    {
      n += 8 - (n % 8);
    }
    this->spec.height = 8 * (n / 8);

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

  uint32_t TileComponent::GetTextureID()
  {
    return quad.GetTextureID();
  }
 
  void TileComponent::UpdateTileData()
  {
    Pixel palette[4] = {{224, 248, 208, 255}, {136, 192, 112, 255}, {52, 104, 86, 255}, {8, 24, 32, 255}};

    GBC::Tile *beg = tile;
    uint32_t roffset = 0;
    uint32_t cap_n = n > 8 ? 8 : n;
    for(uint32_t t = 0; t < n; t++)
    {
      if(t % 8 == 0 && t != 0) roffset += 8 * 8 * 8;
      for(uint8_t y = 0; y <= 7; y++)
      {
	for(uint8_t x = 0; x <= 7; x++)
	{
	  pixels[((y * 8 * cap_n) + x) + ((t % 8) * cap_n) + roffset] = palette[tile->row[y].bpp[x]];
	}
      }
      tile++;
    }

    tile = beg;
    spec.data = (void*)pixels.data();

    quad.UpdateTexture(); 
  }

  void TileComponent::OnUpdate(float dt, const Transform &transform)
  {
    UpdateTileData();
    quad.OnUpdate(dt, transform);
  }
};
