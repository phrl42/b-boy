#include "ecs/components/TileComponent.h"

namespace Banana
{
  TileComponent::TileComponent()
  {
    this->name = "TileComponent";
  }

  TileComponent::TileComponent(GBC::Tile *tile)
  {
    this->tile = tile;
    this->name = "TileComponent";
  }

  void TileComponent::UpdateTile(GBC::Tile *tile)
  {
    this->tile = tile;
  }
 
  void TileComponent::OnUpdate(float dt, const Transform &transform)
  {
    uint8_t x = 0;
    uint8_t y = 0;
    for(uint8_t i = 0; i < 64; i++)
    {
      pixels[i].OnUpdate(dt, {{transform.pos.x + (x * transform.size.x), transform.pos.y + (y * transform.size.y), transform.pos.z}, transform.size, {1, 1, 1, 1}, transform.rotation, transform.proj});

      if(i % 8 == 0)
      {
	y += 1;
	x = 0;
	continue;
      }
      x += 1;
    }
  }
 
};
