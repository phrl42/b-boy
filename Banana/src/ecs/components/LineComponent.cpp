#include "incs.h"
#include "ecs/components/LineComponent.h"
#include "imgui_debug.h"

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

    //if(!Stats::spec->ppu.frame_done) return;
    for(uint8_t y = 0; y < HEIGHT; y++)
    {
      for(uint8_t x = 0; x < WIDTH; x++)
      {
	uint8_t bpp = screen->line[y].fif[x].bpp;
	uint8_t pal = screen->line[y].fif[x].palette;
	
	uint8_t index = 0;
	switch(bpp)
	{
	case 0:
	{
	  index = pal & 0x03;
	  break;
	}

	case 1:
	{
	  index = (pal & 0x0C) >> 2;
	  break;
	}

	case 2:
	{
	  index = (pal & 0x30) >> 4;
	  break;
	}

	case 3:
	{
	  index = (pal & 0xC0) >> 6;
	  break;
	}
	}
	pixels[(y * WIDTH) + x] = palette[index];
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
