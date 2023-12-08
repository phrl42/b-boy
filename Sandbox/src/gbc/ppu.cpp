#include "gbc/ppu.h"
#include "gbc/bus.h"

static int frame = 0;

namespace GBC
{
  PPU::PPU(Interrupt *interrupt)
  {
    this->interrupt = interrupt;

    this->fetch.SCX = &SCX;
    this->fetch.SCY = &SCY;
    
    this->fetch.WX = &WX;
    this->fetch.WY = &WY;

    this->fetch.LCDC = &LCDC;
    this->fetch.LY = &LY;

    this->fetch.screen = &screen;

    this->fetch.tile_data = tile_data;
    
    this->fetch.map1 = map1;
    this->fetch.map2 = map2;

    this->fetch.oam = oam;
  }
  
  uint8_t PPU::Read(uint16_t address)
  {
    if(address <= A_TileDataEND && address >= A_TileData)
    {
      if(rend.mode == Mode::DRAWING_PIXELS) return 0xFF;
      return tile_data[address - A_TileData];
    }
    
    if(address <= A_TileMap1END && address >= A_TileMap1)
    {
      if(rend.mode == Mode::DRAWING_PIXELS) return 0xFF;
      return map1[address - A_TileMap1];
    }

    if(address <= A_TileMap2END && address >= A_TileMap2)
    {
      if(rend.mode == Mode::DRAWING_PIXELS) return 0xFF;
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
      if(rend.mode == Mode::DRAWING_PIXELS) return;
      tile_data[address - A_TileData] = value;
      return;
    }

    if(address <= A_TileMap1END && address >= A_TileMap1)
    {
      if(rend.mode == Mode::DRAWING_PIXELS) return;
      map1[address - A_TileMap1] = value;
      return;
    }

    if(address <= A_TileMap2END && address >= A_TileMap2)
    {
      if(rend.mode == Mode::DRAWING_PIXELS) return;
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

  Tile Fetcher::IndexToTile(uint8_t index, bool BGW)
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

    if(!Get_Bit_N(*LCDC, 4) && BGW)
    {
      //printf("%x\n", start);
      //start = 0x1000 + (int8_t)(index * 16);
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

  uint8_t Fetcher::TileToScreen(uint16_t x, uint16_t y, bool map2)
  {
    if(!Get_Bit_N(*LCDC, 0)) return 3;
    
    uint32_t nt = 0;
    uint16_t ny = y;
    uint16_t nx = x;

    nx += (8 - (x % 8))-1;

    ny += (8 - (y % 8))-1;

    nt = ((((ny / 8))*32) + ((nx / 8))) % 1024;

    nx = x % 8;
    ny = y % 8;

    int index = map2 ? this->map2[nt] : this->map1[nt];

    Tile tile = IndexToTile(index, true);

    return tile.row[ny].bpp[nx];
  }

  Object Fetcher::OAMToObject(uint8_t index)
  {
    Object obj;
    obj.y = oam[index];
    obj.x = oam[index+1];
    obj.index = oam[index+2];
    obj.flags = oam[index+3];

    obj.height = Get_Bit_N(*LCDC, 2) ? 16 : 8;

    return obj;
  }

  // Fetcher

  uint8_t Fetcher::Push(uint8_t rend_x)
  {
    if(fifo_bg.size() <= 8) return 0;

    uint8_t val = fifo_bg.front();
    
    if(val == 2) printf("something bad happened %d {%d : %d}\n", frame, rend_x, *LY);
    
    screen->line[(*LY) % HEIGHT].bpp[rend_x % WIDTH] = val;
    if(!fifo_bg.empty())
    {
      fifo_bg.pop();
    }
    return 1;
  }

  void Fetcher::Read_Tile()
  {
    if(Get_Bit_N(*LCDC, 0))
    {
      x = (x) % 255;
      y = (*LY) % 255;
    }   
    state = Mode::READ_DATA0;
  }

  void Fetcher::Read_Data0()
  {
    state = Mode::READ_DATA1;
  }

  void Fetcher::Read_Data1()
  {
    state = Mode::IDLE;
    if(fifo_bg.size() > 8) return;

    if(x == WIDTH)
    {
      state = Mode::READ_TILE;
      x = 0;
      for(uint8_t i = 0; i < 8; i++)
      {
	fifo_bg.push(2);
      }
      return;
    }
 
    for(uint8_t i = 0; i < 8; i++)
    {
      uint8_t val = 1;//TileToScreen(x + i, y, Get_Bit_N(*LCDC, 3));
      fifo_bg.push(val);
    }
    x += 8;
  }

  void Fetcher::Discard()
  {
    if(fifo_bg.size() <= 0 || fifo_bg.empty()) return;

    std::queue<int> empty = std::queue<int>();
    std::swap(fifo_bg, empty);
    
    state = Mode::READ_TILE;
  }

  void Fetcher::Idle()
  {
    state = Mode::READ_TILE;
  }
  
  void Fetcher::Fetch()
  {
    switch(state)
    {
    case Mode::READ_TILE:
    {
      Read_Tile();
      break;
    }
    
    case Mode::READ_DATA0:
    {
      Read_Data0();
      break;
    }

    case Mode::READ_DATA1:
    {
      Read_Data1();
      break;
    }

    case Mode::IDLE:
    {
      Idle();
      break;
    }

    default:
      GBC_LOG("[PPU] [Fetcher] Invalid State");
      break;
    }

  }

  void PPU::Render()
  {
    if(!Get_Bit_N(LCDC, 7)) return;

    switch(rend.mode)
    {
    case Mode::OAM_SCAN:
    {
      // no oam search for now
      if(rend.dot == P_OAM_END)
      {
	rend.mode = Mode::DRAWING_PIXELS;
      }
      break;
    }

    case Mode::DRAWING_PIXELS:
    {
      if(rend.dot % 2 == 0)
      {
	fetch.Fetch();
      }

      rend.x += fetch.Push(rend.x);
      
      if(rend.dot == 251)
      {
	printf("in frame: %d line has %d dots with element size: %d and x: %d\n", frame, rend.dot, fetch.fifo_bg.size(), rend.x);
	fetch.Discard();
	rend.mode = Mode::HBLANK;
      }
      break;
    }

    case Mode::HBLANK:
    {
      if(rend.dot == P_HBLANK_END)
      {
	LY += 1;

	rend.dot = 0;
	rend.x = 0;
	
	rend.mode = Mode::OAM_SCAN;
	if(LY == HEIGHT)
	{
	  rend.mode = Mode::VBLANK;
	}
      }
      break;
    }

    case Mode::VBLANK:
    {
      if(rend.dot == P_HBLANK_END)
      {
	LY += 1;
      }
      
      if(LY == P_VBLANK_END)
      {
	LY = 0;
	//fetch.state = Fetcher::Mode::NONE;
	rend.mode = Mode::OAM_SCAN;
	frame += 1;
      }
      break;
    }

    default:
      GBC_LOG("[PPU] Rendering Mode invalid");
      break;
    }

    rend.dot++;
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
      tmap1[i] = fetch.IndexToTile(map1[i], false);
    }

    for(uint16_t i = 0; i < 32*32; i++)
    {
      tmap2[i] = fetch.IndexToTile(map2[i], false);
    }
  }

  void PPU::UpdateOAM()
  {
    for(uint8_t i = 0; i < 40; i++)
    {
      OAM_tiles[i] = fetch.IndexToTile(objects[i].index, false);
    }
  }
  

};
