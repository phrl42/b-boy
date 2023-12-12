#include "gbc/ppu.h"
#include "gbc/bus.h"

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
    // 1 tile = 16 byte
    Tile temp = {0};

    uint16_t start = index * 16;

    if(!Get_Bit_N(*LCDC, 4) && BGW)
    {
      start = 0x1000 + (int8_t)(index) * 16;
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

  uint8_t Fetcher::TileToScreen(uint8_t x, uint8_t y, bool map2)
  {
    uint32_t nt = 0;
    uint16_t ny = y % 8;
    uint16_t nx = x % 8;

    nt = (x / 8) + ((y / 8) * 32) % 1024;

    int index = map2 ? this->map2[nt] : this->map1[nt];
    Tile tile = IndexToTile(index, tile_mode == TileMode::OBJ ? false : true);

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

  void Fetcher::Pop()
  {
    if(tile_mode != TileMode::OBJ)
    {
      for(uint8_t i = 0; i < 7; i++)
      {
	fifo_bg[i] = fifo_bg[i+1];
      }
      bg_size--;
    }
    else if(tile_mode == TileMode::OBJ)
    {
      for(uint8_t i = 0; i < 7; i++)
      {
	fifo_obj[i] = fifo_obj[i+1];
      }
      obj_size--;
    }
  }

  uint8_t Fetcher::Push(uint8_t rend_x)
  {
    if(tile_mode != TileMode::OBJ)
    {
      if(bg_size == 0) return 0;
      screen->line[(*LY) % HEIGHT].bpp[rend_x] = fifo_bg[0];
    }
    else if(tile_mode == TileMode::OBJ)
    {
      if(obj_size == 0) return 0;
      screen->line[(*LY) % HEIGHT].bpp[rend_x] = fifo_obj[0];
    }
    
    Pop();

    return 1;
  }

  void Fetcher::Read_Tile()
  {
    bool sprite_check = false;

    for(uint8_t i = 0; i < 8; i++)
    {
      if(buffer[i].x <= (x + 8) && buffer[i].x != 0)
      {
	sprite_check = true;
      }
    }
    
    /*if(sprite_check)
    {
      x = x;
      y = *LY;
      tile_mode = TileMode::OBJ;
      }*/
    /*else if(Get_Bit_N(*LCDC, 5) && window_trigger && x >= (*WX - 7))
    {
      static bool first = false;

      if(first)
      {
	for(uint8_t i = 0; i < 8; i++)
	{
	  fifo_bg[i] = 0;
	  fetch[i] = 0;
	}
	bg_size = 0;
	x = 0;
	first = true;
      }
      x = x + *WX;
      y = *LY;
      tile_mode = TileMode::W;
      }*/
    if(Get_Bit_N(*LCDC, 0))
    {
      if(line_begin)
      {
	x = *SCX;
	line_begin = false;
      }

      y = *LY + *SCY;

      tile_mode = TileMode::BG;
    }
    state = Mode::READ_DATA0;
  }

  void Fetcher::Read_Data0()
  {
    state = Mode::READ_DATA1;
  }

  void Fetcher::Read_Data1()
  {
    if(start)
    {
      start = false;
      state = Mode::READ_TILE;
      return;
    }
    
    for(uint8_t i = 0; i < 8; i++)
    {
      if(tile_mode == TileMode::OBJ) fetch_obj[i] = TileToScreen(x+i, y, Get_Bit_N(*LCDC, 3));
      if(tile_mode == TileMode::BG) fetch[i] = TileToScreen(x+i, y, Get_Bit_N(*LCDC, 3));
      if(tile_mode == TileMode::W) fetch[i] = TileToScreen(x+i, y, Get_Bit_N(*LCDC, 6));
    }
    x += 8; 
    
    state = Mode::PUSH_FIFO;
  }

  void Fetcher::Push_FIFO()
  {
    if(bg_size == 0 && tile_mode != TileMode::OBJ)
    {
      for(uint8_t i = 0; i < 8; i++)
      {
	fifo_bg[i] = fetch[i];
      }
      bg_size = 8;
      state = Mode::READ_TILE;
    }
    else if(obj_size == 0 && tile_mode == TileMode::OBJ)
    {
      for(uint8_t i = 0; i < 8; i++)
      {
	fifo_obj[i] = fetch_obj[i];
      }
      obj_size = 8;
      state = Mode::READ_TILE;
    }
  }

  void Fetcher::Discard()
  {
    for(uint8_t i = 0; i < 8; i++)
    {
      fifo_bg[i] = 0;
      fetch[i] = 0;
      
      fetch_obj[i] = 0;
      fifo_obj[i] = 0;
    }
    bg_size = 0;
    obj_size = 0;
    state = Mode::READ_TILE;
    tile_mode = TileMode::NONE;

    line_begin = true;
  }

  void Fetcher::Reset()
  {
    Discard();
    x = 0;
    y = 0;
    start = true;
    scx_done = false;
    tile_mode = TileMode::NONE;
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

    case Mode::PUSH_FIFO:
    {
      Push_FIFO();
      break;
    }

    default:
      GBC_LOG("[PPU] [Fetcher] Invalid State");
      break;
    }

  }

  void PPU::Draw_Pixel()
  {
    if(rend.x == P_DRAW_END || rend.dot == 80) return;
    
    if(rend.dot % 2 == 0)
    {
      fetch.Fetch();
    }

    rend.x += fetch.Push(rend.x);
  }

  void PPU::Render()
  {
    if(!Get_Bit_N(LCDC, 7)) return;
    
    switch(rend.mode)
    {
    case Mode::OAM_SCAN:
    {
      
      if(!fetch.window_trigger)
      {
	fetch.window_trigger = (bool)(LY == WY);
      }
      frame_done = false;
      // set PPU Mode
      Set_Bit_N(&STAT, 0, 0);
      Set_Bit_N(&STAT, 1, 1);

      if(rend.dot % 2 == 0)
      {
	uint8_t index = rend.dot / 2;

	objects[index] = fetch.OAMToObject(index * 4);
	
	if(objects[index].x > 0 && (LY + 16) >= objects[index].y && (LY + 16) < (objects[index].height + objects[index].y) && fetch.sprite_size < 10)
	{
	  fetch.buffer[fetch.sprite_size] = objects[index];
	  fetch.sprite_size++;
	}
      }

      if(rend.dot == P_OAM_END)
      {
	rend.mode = Mode::DRAWING_PIXELS;
      }
      break;
    }

    case Mode::DRAWING_PIXELS:
    {
      frame_done = false;
      // set PPU Mode
      Set_Bit_N(&STAT, 0, 1);
      Set_Bit_N(&STAT, 1, 1);

      Draw_Pixel();
      if(rend.x == P_DRAW_END)
      {
	fetch.Reset();
	rend.mode = Mode::HBLANK;
      }
      break;
    }

    case Mode::HBLANK:
    {
      frame_done = true;
      // set PPU Mode
      Set_Bit_N(&STAT, 0, 0);
      Set_Bit_N(&STAT, 1, 0);

      if(rend.dot == P_HBLANK_END)
      {
	LY += 1;
	
	line_interrupt_done = false;
	Set_Bit_N(&STAT, 2, 0); 

	rend.dot = 0;
	rend.x = 0;
	rend.mode = Mode::OAM_SCAN;
	if(LY == HEIGHT)
	{
	  rend.mode = Mode::VBLANK;
	  interrupt->Request(INTERRUPT::VBLANK);
	}
      }
      break;
    }

    case Mode::VBLANK:
    {
      frame_done = true;
      fetch.window_line_counter = 0;
      
      // set PPU Mode
      Set_Bit_N(&STAT, 0, 1);
      Set_Bit_N(&STAT, 1, 0);

      if(Get_Bit_N(STAT, 4) && line_interrupt_done == false)
      {
	line_interrupt_done = true;
	interrupt->Request(INTERRUPT::LCD);
      }
      
      if(rend.dot == P_HBLANK_END)
      {
	LY += 1;
	rend.dot = 0;
	rend.x = 0;
	line_interrupt_done = false;
	Set_Bit_N(&STAT, 2, 0);
      }
      
      if(LY == P_VBLANK_END)
      {
	LY = 0;
	rend.dot = 0;
	rend.x = 0;
	line_interrupt_done = false;
	Set_Bit_N(&STAT, 2, 0);

	rend.mode = Mode::OAM_SCAN;

	frames++;
	fetch.window_trigger = false;
      }
      break;
    }

    default:
      GBC_LOG("[PPU] Rendering Mode invalid");
      break;
    }

    if(Get_Bit_N(STAT, 6) && !Get_Bit_N(STAT, 2) && LYC == LY && line_interrupt_done == false)
    {
      Set_Bit_N(&STAT, 2, 1);
      line_interrupt_done = true;
      interrupt->Request(INTERRUPT::LCD);
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
