#include "ecs/components/TileComponent.h"

namespace Banana
{
  TileComponent::TileComponent()
  {
    this->name = "TileComponent";

    // init spec and give quadcomponent spec
  }

  TileComponent::TileComponent(GBC::Tile *tile)
  {
    this->tile = tile;
    TileComponent();
  }

  void TileComponent::UpdateTile(GBC::Tile *tile)
  {
    this->tile = tile;
  }
 
  void TileComponent::OnUpdate(float dt, const Transform &transform)
  {
    uint8_t x = 0;
    uint8_t y = 7;

    glm::vec4 palette[4] = {{0.498, 0.525, 0.059, 1}, {0.341, 0.486, 0.267, 1}, {0.212, 0.365, 0.282, 1}, {0.165, 0.271, 0.231, 1}};

    float one_width = transform.size.x / 8;
    float one_height = transform.size.y / 8;
    
    for(uint8_t i = 0; i < 64; i++)
    {
      if(i % 8 == 0 && i != 0)
      {
	y -= 1;
	x = 0;
      }

      // modify spec pixeldata
      //pixels[i].OnUpdate(dt, {{transform.pos.x + (x * one_width), transform.pos.y + (y * one_height), transform.pos.z}, {one_width, one_height, transform.size.z}, palette[tile->row[7 - y].bpp[x]], transform.rotation, transform.proj});
      x += 1;
    }

    // give quadcomponent new spec with modified pixel data

    // enjoy
  }
 
};
