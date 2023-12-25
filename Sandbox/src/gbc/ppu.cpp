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

    this->fetch.OBP0 = &OBP0;
    this->fetch.OBP1 = &OBP1;

    this->fetch.BGP = &BGP;
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

  Tile Fetcher::IndexToTile(uint8_t index, bool BGW, bool hflip, bool vflip)
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

    if(hflip)
    {
      Tile tile_h = {0};
      for(uint8_t y = 0; y < 8; y++)
      {
	for(uint8_t x = 0; x < 8; x++)
	{
	  tile_h.row[y].bpp[x] = temp.row[y].bpp[7 - x];
	}
      }
      temp = tile_h;
    }

    if(vflip)
    {
      Tile tile_v = {0};
      for(uint8_t y = 0; y < 8; y++)
      {
	for(uint8_t x = 0; x < 8; x++)
	{
	  tile_v.row[y].bpp[x] = temp.row[7 - y].bpp[x];
	}
      }
      temp = tile_v;
    }

    return temp;
  }

  void Fetcher::PreLoadTiles(uint8_t y)
  {
    bool map2 = Get_Bit_N(*LCDC, 3);
    bool map2_w = Get_Bit_N(*LCDC, 6);
    
    for(size_t i = 0; i < 160; i += 8)
    {
      uint32_t nt = 0;

      nt = (i / 8) + ((y / 8) * 32) % 1024;

      int index = map2 ? this->map2[nt] : this->map1[nt];
      Tile tile = IndexToTile(index, true);

      tile_line[i/8] = tile;
    }
    
  }

  uint8_t Fetcher::TileToScreen(uint8_t x, uint8_t y, bool map2)
  {
    uint32_t nt = 0;
    uint16_t ny = y % 8;
    uint16_t nx = x % 8;

    nt = (x / 8) + ((y / 8) * 32) % 1024;

    int index = map2 ? this->map2[nt] : this->map1[nt];

    uint16_t start = index * 16;

    if(!Get_Bit_N(*LCDC, 4))
    {
      start = 0x1000 + (int8_t)(index) * 16;
    }

    uint16_t lower_row = tile_data[start + (2 * ny)];
    uint16_t higher_row = tile_data[start+1 + (2 * ny)];

    uint8_t res_bit = 0;

    uint8_t lower_bit = Get_Bit_N(lower_row, 7 - (nx));
    uint8_t higher_bit = Get_Bit_N(higher_row, 7 - (nx));

    res_bit = higher_bit << 1 | lower_bit;

    return res_bit;
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
    for(uint8_t i = 0; i < 7; i++)
    {
      fifo_bg[i] = fifo_bg[i+1];
    }
    bg_size--;
  }

  uint8_t Fetcher::Push(uint8_t rend_x)
  {
    if(bg_size == 0) return 0;
    FIFO fif;
    fif.bpp = fifo_bg[0].bpp;
    fif.palette = fifo_bg[0].palette;

    if(Get_Bit_N(*LCDC, 1))
    {
      FIFO obj_fif = sprite_line[rend_x];
    
      uint8_t spp = obj_fif.bpp;
      uint8_t pal = obj_fif.palette;
 
      fif.bpp = spp;
      fif.palette = pal;
     
      if(sprite_line[rend_x].bg_prio && fifo_bg[0].bpp != 0)
      {
	fif.bpp = fifo_bg[0].bpp;
	fif.palette = fifo_bg[0].palette;
      }

      if(obj_fif.bpp == 5 || obj_fif.bpp == 0)
      {
	fif.bpp = fifo_bg[0].bpp;
	fif.palette = fifo_bg[0].palette;
      }
    
    }
    
    screen->line[(*LY) % HEIGHT].fif[rend_x] = fif;

    Pop();
    
    return 1;
  }

  void Fetcher::Read_Tile()
  {
    if(Get_Bit_N(*LCDC, 5) && window_trigger && (x + 7) >= (*WX) || tile_mode == TileMode::W)
    {
      if(window_begin)
      {
	w_x = 0;
	w_y = window_line_counter;
	window_begin = false;
      }

      tile_mode = TileMode::W;
    }
    else if(Get_Bit_N(*LCDC, 0))
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
      if((tile_mode == TileMode::BG) && window_begin)
      {
	FIFO fif;
	fif.bpp = TileToScreen(x+i, y, Get_Bit_N(*LCDC, 3));
	fif.palette = *BGP;
	fetch[i] = fif;
      }
      if((tile_mode == TileMode::W) && !window_begin)
      {
	FIFO fif;
	fif.bpp = TileToScreen(w_x+i, w_y, Get_Bit_N(*LCDC, 6));
	fif.palette = *BGP;
	fetch[i] = fif;
      }
    }

    x += 8;
    if(!window_begin) w_x += 8;
    state = Mode::PUSH_FIFO;
  }

  void Fetcher::Push_FIFO()
  {
    if(bg_size == 0)
    {
      for(uint8_t i = 0; i < 8; i++)
      {
	fifo_bg[i] = fetch[i];
      }
      bg_size = 8;
      state = Mode::READ_TILE;
    }
  }

  void Fetcher::Discard()
  {
    for(uint8_t i = 0; i < 8; i++)
    {
      fifo_bg[i] = FIFO();
      fetch[i] = FIFO();
    }
    bg_size = 0;
    sprite_size = 0;
    state = Mode::READ_TILE;

    if(tile_mode == TileMode::W) window_line_counter++;

    tile_mode = TileMode::NONE;

    window_begin = true;
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

  // consumes a lot of resources
  void PPU::Render()
  {
    if(!Get_Bit_N(LCDC, 7))
    {
      Set_Bit_N(&STAT, 0, 0);
      Set_Bit_N(&STAT, 1, 0);
      return;
    }
    
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
      }

      if(rend.dot == P_OAM_END)
      {
	// sort oam
	for(uint8_t i = 0; i < 40; i++)
	{
	  for(uint8_t j = 0; j < 40; j++)
	  {
	    Object obj;
	    if(objects[i].x < objects[j].x)
	    {
	      obj = objects[i];
	      objects[i] = objects[j];
	      objects[j] = obj;
	    }
	  }
	}

	// filter sprites
	for(uint8_t i = 0; i < 40; i++)
	{
	  if(objects[i].x > 0 && objects[i].x <= 160 && (LY + 16) >= objects[i].y && (LY + 16) < (objects[i].height + objects[i].y) && fetch.sprite_size < 10)
	  {
	    fetch.buffer[fetch.sprite_size] = objects[i];
	    fetch.sprite_size++;
	  }
	}

	// initialize sprite line
	for(uint16_t x = 0; x < WIDTH; x++)
	{
	  FIFO fif;
	  fif.bpp = 5;
	  fif.palette = 0;
	  fif.bg_prio = 0;

	  fetch.sprite_line[x] = fif;
	}

	// put sprites into line
	for(int s = fetch.sprite_size-1; s >= 0; s--)
	{
	  Object obj = fetch.buffer[s];
	  if(s != 0)
	  {
	    if(fetch.buffer[s-1].x == obj.x)
	    {
	      if(obj.index > fetch.buffer[s-1].index)
	      {
		fetch.buffer[s-1] = obj;
	      }
		
	      continue;
	    }
	  }

	  for(uint8_t i = 0; i < 8; i++)
	  {
	    FIFO fif;
	    fif.bg_prio = Get_Bit_N(obj.flags, 7);
	    fif.palette = Get_Bit_N(obj.flags, 4) ? OBP1 : OBP0;

	    // sprite pixel depends on flip state
	    bool hflip = Get_Bit_N(obj.flags, 5);
	    bool vflip = Get_Bit_N(obj.flags, 6);

	    uint8_t y = ((LY - ((obj.y)-16)) % 8); 

	    // object index gets modified
	    uint16_t index = obj.index;
	  
	    if(obj.height == 16 && !(8 <= (LY - (obj.y-16))))
	    {
	      obj.index = obj.index & 0xFE;
	    }
	    else if(8 <= (LY - (obj.y-16)) && obj.height == 16)
	    {
	      obj.index = obj.index | 0x01;
	    }

	    if(obj.height == 16 && Get_Bit_N(obj.flags, 6) && !(8 <= (LY - (obj.y-16))))
	    {
	      obj.index = index | 0x01;
	    }
	    else if(obj.height == 16 && 8 <= (LY - (obj.y-16)) && Get_Bit_N(obj.flags, 6))
	    {
	      obj.index = index & 0xFE;
	    }
	    
	    fif.bpp = fetch.IndexToTile(obj.index, false, hflip, vflip).row[y].bpp[i];

	    if(fif.bpp == 0 && fetch.sprite_line[(obj.x-8)+i].bpp != 0) continue;
	    fetch.sprite_line[(obj.x-8)+i] = fif;
	  }
	}
	//fetch.PreLoadTiles(LY);
	
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

	rend.dot = -1;
	rend.x = 0;
	rend.mode = Mode::OAM_SCAN;
	if(LY == HEIGHT)
	{
	  rend.mode = Mode::VBLANK;
	  fetch.window_line_counter = 0;
	  interrupt->Request(INTERRUPT::VBLANK);
	}
      }
      break;
    }

    case Mode::VBLANK:
    {
      frame_done = true;
      
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
	rend.dot = -1;
	rend.x = 0;
	line_interrupt_done = false;
	Set_Bit_N(&STAT, 2, 0);
      }
      
      if(LY == P_VBLANK_END)
      {
	LY = 0;
	rend.dot = -1;
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
