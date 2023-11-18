#include "ecs/components/TileComponent.h"

namespace Banana
{
  TileComponent::TileComponent(GBC::Tile (&tile)[], uint32_t n)
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
    
    this->spec.size = spec.width * spec.height * 3;
    this->spec.format = ImageFormat::RGB8;

    this->quad = QuadComponent(&spec);

    UpdateTileData();
  }

  uint32_t TileComponent::GetTextureID()
  {
    return quad.GetTextureID();
  }
 
  void TileComponent::UpdateTileData()
  {

    Pixel palette[4] = {{224, 248, 208}, {136, 192, 112}, {52, 104, 86}, {8, 24, 32}};

    for(uint32_t t = 0; t < n; t++)
    {
      uint8_t x = 0;
      uint8_t y = 7;
  
      for(uint8_t i = 0; i < 64; i++)
      {
	if(i % 8 == 0 && i != 0)
	{
	  y -= 1;
	  x = 0;
	}
	pixels[(((7 - y) * 8) + x) + ((t) * 64)] = palette[tile[t].row[7 - y].bpp[x]]; 
	x += 1;
      }
    }
    spec.data = (void*)pixels;

    quad.UpdateTexture(); 
  }

  void TileComponent::UpdateTile(GBC::Tile (&tile)[])
  {
  }
 
  void TileComponent::OnUpdate(float dt, const Transform &transform)
  {
    UpdateTileData();
    quad.OnUpdate(dt, transform);
  }
};
