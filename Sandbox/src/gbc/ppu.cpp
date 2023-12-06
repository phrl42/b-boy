#include "gbc/ppu.h"
#include "gbc/bus.h"

namespace GBC
{
  PPU::PPU()
  {

  }
  
  uint8_t PPU::Read(uint16_t address)
  {
    if(address <= A_TileDataEND && address >= A_TileData)
    {
      if(rend.mode == Mode::THREE) return 0xFF;
      return tile_data[address - A_TileData];
    }
    
    if(address <= A_TileMap1END && address >= A_TileMap1)
    {
      if(rend.mode == Mode::THREE) return 0xFF;
      return map1[address - A_TileMap1];
    }

    if(address <= A_TileMap2END && address >= A_TileMap2)
    {
      if(rend.mode == Mode::THREE) return 0xFF;
      return map2[address - A_TileMap2]; 
    }

    if(address <= A_OAMEND && address >= A_OAM)
    {
      return oam[address - A_OAM];
    }

    switch(address)
    {
    case A_OAM:
      break;

    case A_LCDC:
      return LCDC;
      break;

    case A_STAT:
      return STAT;
      break;

    case A_SCX:
      return SCX;
      break;

    case A_SCY:
      return SCY;
      break;

    case A_LY:
      return LY;
      break;

    case A_LYC:
      return LYC;
      break;

    case A_DMA:
      return DMA;
      break;

    case A_BGP:
      return BGP;
      break;

    case A_OBP0:
      return OBP0;
      break;

    case A_OBP1:
      return OBP1;
      break;

    case A_WX:
      return WX;
      break;

    case A_WY:
      return WY;
      break;

    default:
      GBC_LOG("[PPU]: WRONG READ");
    }

    return 0;
  }
  
  void PPU::Write(uint16_t address, uint8_t value)
  {
    if(address <= A_TileDataEND && address >= A_TileData)
    {
      if(rend.mode == Mode::THREE) return;
      tile_data[address - A_TileData] = value;
      return;
    }

    if(address <= A_TileMap1END && address >= A_TileMap1)
    {
      if(rend.mode == Mode::THREE) return;
      map1[address - A_TileMap1] = value;
      return;
    }

    if(address <= A_TileMap2END && address >= A_TileMap2)
    {
      if(rend.mode == Mode::THREE) return;
      map2[address - A_TileMap2] = value;
      return;
    }

    if(address <= A_OAMEND && address >= A_OAM)
    {
      oam[address - A_OAM] = value;
      return;
    }

    switch(address)
    {
    case A_LCDC:
      LCDC = value;
      break;

    case A_STAT:
      STAT = value;
      break;

    case A_SCX:
      SCX = value;
      break;

    case A_SCY:
      SCY = value;
      break;

    case A_LY:
      LY = value;
      break;

    case A_LYC:
      LYC = value;
      break;

    case A_DMA:
      DMA = value;
      break;

    case A_BGP:
      BGP = value;
      break;

    case A_OBP0:
      OBP0 = value;
      break;

    case A_OBP1:
      OBP1 = value;
      break;

    case A_WX:
      WX = value;
      break;

    case A_WY:
      WY = value;
      break;

    default:
      GBC_LOG("[PPU]: WRONG WRITE");
    }

    return;
  }

  void PPU::UpdateSettings()
  {
    
  }

  uint8_t PPU::TileToScreen(uint16_t x, uint16_t y, bool map2)
  {
    uint32_t nt = 0;
    uint16_t ny = y;
    uint16_t nx = x;

    nx += (8 - (x % 8))-1;

    ny += (8 - (y % 8))-1;

    nt = (((ny / 8))*32) + ((nx / 8));

    nx = x % 8;
    ny = y % 8;

    int index = map2 ? this->map2[nt] : this->map1[nt];

    Tile tile = IndexToTile(index, true);
    
    return tile.row[ny].bpp[nx];
  }

  Object PPU::OAMToObject(uint8_t index)
  {
    Object obj;
    obj.y = oam[index];
    obj.x = oam[index+1];
    obj.index = oam[index+2];
    obj.flags = oam[index+3];

    obj.height = Get_Bit_N(LCDC, 2) ? 16 : 8;

    return obj;
  }

  // do X,Y logic here
  void PPU::Step1()
  {
    // draw bg
    if(Get_Bit_N(LCDC, 0))
    {
      // map2 bool may cause bugs here
      // unclear when to set true
      rend.pixfetcher.x = (SCX + rend.x) % 255;
      rend.pixfetcher.y = (SCY + LY) % 255;
    }

    // draw window
    if(Get_Bit_N(LCDC, 5) && Get_Bit_N(LCDC, 0))
    {
      rend.pixfetcher.x = WX;
      rend.pixfetcher.y = WY;
    }

    // draw obj
    if(Get_Bit_N(LCDC, 1))
    {

    }
	
    rend.pixfetcher.current_step = 2;
  }

  void PPU::Step2()
  {
    rend.pixfetcher.current_step = 3;
  }

  void PPU::Step3()
  {
    rend.pixfetcher.current_step = 4;

    if(rend.pixfetcher.fifo_bg.size() <= 8)
    {
      for(uint8_t i = 0; i < 8; i++)
      {
	FIFO fif;
	fif.bpp = TileToScreen(rend.pixfetcher.x+i, rend.pixfetcher.y, Get_Bit_N(LCDC, 3));

	rend.pixfetcher.fifo_bg.push(fif);
      }
	    
    }

  }
  // todo follow ultimate gameboy talk fifo guide
  void PPU::Step4()
  {
    if(rend.pixfetcher.fifo.bg_size() <= 8)
    {
      rend.pixfetcher.current_step = 3;
      return;
    }

    rend.pixfetcher.current_step = 1;
  }

  void PPU::Push()
  {
    screen.line[LY].bpp[(rend.x-8) + i] = rend.pixfetcher.fifo_bg.front().bpp;
    rend.pixfetcher.fifo_bg.pop();
  }
  
  // progresses one dot
  void PPU::Render()
  {
    if(!Get_Bit_N(LCDC, 7)) return;
    if(LY == 0 && rend.mode == Mode::ONE) rend.mode = Mode::TWO;
    
    if(rend.mode == Mode::TWO)
    {
      if(rend.dot % 2 == 0)
      {
	Object obj = OAMToObject(rend.dot / 2);

	if(obj.x > 0 && obj.y <= (LY+16) && (LY+16) < (obj.y + obj.height) && rend.buffer_index < 10)
	{
	  rend.buffer[rend.buffer_index] = obj;
	  rend.buffer_index++;
	}
      }
    }
    
    if(rend.mode == Mode::THREE)
    {
      if(rend.dot % 2 == 0)
      {
	switch(rend.pixfetcher.current_step)
	{
	case 1:
	  Step1();
	  break;

	case 2:
	  Step2();
	  break;

	case 3:
	  Step3();
	  break;

	case 4:
	  Step4();
	  break;

	default:
	  GBC_LOG("[PPU] Step Invalid");
	  break;
	}
      }
     
      rend.x += 1;
    }
    
    rend.dot += 1;
    if(rend.dot == 80 && rend.mode == Mode::TWO) rend.mode = Mode::THREE;
    if(rend.x == WIDTH) rend.mode = Mode::ZERO;
    if(rend.dot == 456)
    {
      LY += 1;
      rend.x = 0;
      rend.dot = 0;

      for(uint8_t i = 0; i <= rend.buffer_index; i++)
      {
	rend.buffer[i].x = 0;
	rend.buffer[i].y = 0;
	rend.buffer[i].index = 0;
	rend.buffer[i].flags = 0;
      }
      rend.buffer_index = 0;
      
      if(LY < HEIGHT) rend.mode = Mode::TWO;
    }
    if(LY == HEIGHT)
    {
      rend.mode = Mode::ONE;
    }
    if(LY == 154)
    {
      LY = 0;
      rend.mode = Mode::TWO;
    }
  }

  // called every t-cycle
  void PPU::Tick()
  {
    Render();
  }
  
  void PPU::DMATransfer(uint8_t *chunk)
  {
    for(uint8_t byte = 0; byte < 0xA0; byte++)
    {
      oam[byte] = *chunk;
      chunk++;
    }
  }

  Tile PPU::IndexToTile(uint8_t index, bool BGW)
  {
    /*tile_data[0] = 0x3C;
      tile_data[1] = 0x7E;
      tile_data[2] = 0x42;
      tile_data[3] = 0x42;
      tile_data[4] = 0x42;
      tile_data[5] = 0x42;
      tile_data[6] = 0x42;
      tile_data[7] = 0x42;
      tile_data[8] = 0x7E;
      tile_data[9] = 0x5E;
      tile_data[10] = 0x7E;
      tile_data[11] = 0x0A;
      tile_data[12] = 0x7C;
      tile_data[13] = 0x56;
      tile_data[14] = 0x38;
      tile_data[15] = 0x7C;*/
    // 1 tile = 16 byte
    Tile temp = {0};

    uint16_t start = index * 16;

    if(!Get_Bit_N(LCDC, 4) && BGW)
    {
      //start = 0x1000 + (int8_t)((int8_t)(index) * 16);
    }

    int row_index = 0;
    for(uint16_t pos = start; pos <= start+15; pos += 2)
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
	temp.row[row_index].bpp[i] = res_bit;
      }
      row_index++;
    }

    return temp;
  }

  // debugging only

  void PPU::UpdateTiles()
  {
    int row_index = 0;
    int tile_index = 0;
    for(uint16_t pos = 0; pos <= A_TileDataEND - A_TileData; pos += 2)
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

  void PPU::UpdateMaps()
  {
    for(uint16_t i = 0; i < 32*32; i++)
    {
      tmap1[i] = IndexToTile(map1[i], false);
    }

    for(uint16_t i = 0; i < 32*32; i++)
    {
      tmap2[i] = IndexToTile(map2[i], false);
    }
  }

  void PPU::UpdateOAM()
  {
    for(uint8_t i = 0; i < 40; i++)
    {
      OAM_tiles[i] = IndexToTile(objects[i].index, false);
    }
  }
  

};
