#include "gbc/ppu.h"

namespace GBC
{
  uint8_t Get_Bit_N(uint8_t src, uint8_t n)
  {
    if(n > 8)
    {
      GBC_LOG("Could not get bit. n > 8");
      return 0;
    }

    src <<= 7 - n;
    src >>= 7;

    return src;
  }

  PPU::PPU()
  {

  }
  
  void PPU::Read(uint16_t address)
  {

  }
  
  void PPU::Write(uint16_t address, uint8_t value)
  {

  }
 
  void PPU::Render()
  {
    UpdateTiles();
  }

  void PPU::UpdateTiles()
  {
    const uint16_t LIMIT = 0x97FF;

    // 1 tile = 16 byte

    int row_index = 0;
    int tile_index = 0;
    for(uint16_t pos = A_TileData; pos < LIMIT; pos += 2)
    {
      uint16_t lower_row = 0;//bus->Read(pos);
      uint16_t higher_row = 0;//bus->Read(pos+1);
      
      for(uint8_t i = 0; i <= 7; i++)
      {
	uint8_t res_bit = 0;

	uint8_t lower_bit = Get_Bit_N(lower_row, 7 - i);
	uint8_t higher_bit = Get_Bit_N(higher_row, 7 - i);

	// reverse order is required by gpu
	res_bit = higher_bit << 1 | lower_bit;
	tile[tile_index].row[row_index].bpp[i] = res_bit;
      }

      if(row_index == 7)
      {
	row_index = 0;
	tile_index++;
	continue;
      }
      row_index++;
    }
  }

};
