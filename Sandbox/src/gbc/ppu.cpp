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
  
  uint8_t PPU::Read(uint16_t address)
  {
    if(address <= A_TileDataEND && address >= A_TileData)
    {
      return tile_data[address - A_TileData];
    }
    
    if(address <= A_TileMap1END && address >= A_TileMap1)
    {
      return map1[address - A_TileMap1];
    }

    if(address <= A_TileMap2END && address >= A_TileMap2)
    {
      return map2[address - A_TileMap2]; 
    }
    
    switch(address)
    {
    case A_OAM:
      break;

    case A_LCDC:
      break;

    case A_STAT:
      break;

    case A_SCX:
      break;

    case A_SCY:
      break;

    case A_LY:
      return 0x90;
      break;

    case A_LYC:
      break;

    case A_DMA:
      break;

    case A_BGP:
      break;

    case A_OBP0:
      break;

    case A_OBP1:
      break;

    case A_WX:
      break;

    case A_WY:
      break;

    default:
      GBC_LOG("[PPU]: WRONG READ");
    }
  }
  
  void PPU::Write(uint16_t address, uint8_t value)
  {
    if(address <= A_TileDataEND && address >= A_TileData)
    {
      tile_data[address - A_TileData] = value;
      return;
    }

    if(address <= A_TileMap1END && address >= A_TileMap1)
    {
      map1[address - A_TileMap1] = value;
      return;
    }

    if(address <= A_TileMap2END && address >= A_TileMap2)
    {
      map2[address - A_TileMap2] = value;
      return;
    }

    switch(address)
    {
    case A_OAM:
      break;

    case A_LCDC:
      break;

    case A_STAT:
      break;

    case A_SCX:
      break;

    case A_SCY:
      break;

    case A_LY:
      break;

    case A_LYC:
      break;

    case A_DMA:
      break;

    case A_BGP:
      break;

    case A_OBP0:
      break;

    case A_OBP1:
      break;

    case A_WX:
      break;

    case A_WY:
      break;

    default:
      GBC_LOG("[PPU]: WRONG WRITE");
    }
  }
 
  void PPU::Render()
  {
    UpdateTiles();
  }

  void PPU::UpdateTiles()
  {
    // 1 tile = 16 byte

    int row_index = 0;
    int tile_index = 0;
    for(uint16_t pos = 0; pos < A_TileDataEND - A_TileData; pos += 2)
    {
      uint16_t lower_row = tile_data[pos];
      uint16_t higher_row = tile_data[pos+1];
      
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
