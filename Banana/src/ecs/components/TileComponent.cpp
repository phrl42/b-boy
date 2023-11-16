#include "ecs/components/TileComponent.h"

namespace Banana
{
  TileComponent::TileComponent()
  {

  }

  TileComponent::TileComponent(GBC::Tile *tile)
  {
    this->tile = tile;
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
      pixels[i].OnUpdate(dt, {{transform.pos.x, transform.pos.y, transform.pos.z}, transform.size, {1, 1, 1, 1}, transform.rotation, transform.proj});

      if(i == 7)
      {
	y += 1;
	x = 0;
      }
      x += 1;
    }
  }
 
};
