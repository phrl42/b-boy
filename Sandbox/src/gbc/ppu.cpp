#include "gbc/ppu.h"

namespace GBC
{

  void PPU::Render()
  {
    
    UpdateTiles();
  }

  void PPU::UpdateTiles()
  {
    const uint16_t LIMIT = 0x97FF;

    // 1 tile = 16 byte

    for(uint16_t pos = TileData; pos < LIMIT; pos++)
    {
      //tile[pos] = 
    }
  }

};
