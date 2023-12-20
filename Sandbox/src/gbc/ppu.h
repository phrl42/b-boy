#pragma once
#include "Sandbox.h"
#include "gbc/bitwise.h"
#include "gbc/interrupt.h"

#include <queue>

#define WIDTH 160
#define HEIGHT 144

#define P_OAM_END 79
#define P_DRAW_END 160
#define P_HBLANK_END 456
#define P_VBLANK_END 154

// PPU Registers are located in RAM
// Tile Data Location
#define A_TileData 0x8000
#define A_TileDataEND 0x97FF

// 2 32x32 tile maps
#define A_TileMap1 0x9800
#define A_TileMap1END 0x9BFF

#define A_TileMap2 0x9C00
#define A_TileMap2END 0x9FFF

// Object Attribute Map
#define A_OAM 0xFE00
#define A_OAMEND 0xFE9F
// 4 byte space 
    
// LCD Control (R/W)
#define A_LCDC 0xFF40
// 7 LCD Enable
// 6 Window Tile Map
// 5 Window Enable
// 4 BG and W Tile Map
// 3 BG Tile Map
// 2 OBJ Size
// 1 OBJ Enable
// 0 BG and W Enable
    
// LCD Status (R/W)
#define A_STAT 0xFF41
// 6 LYC=LY Interrupt
// 5 Mode 2 OAM Interrupt
// 4 Mode 1 V-Blank Interrupt
// 3 Mode 0 H-Blank Interrupt
// 2 LYC=LY Flag
// 1-0 Mode 
         
// Scroll X / Y
#define A_SCY 0xFF42
#define A_SCX 0xFF43

// LCDC Y Coordinate
#define A_LY 0xFF44

// LY Compare
#define A_LYC 0xFF45

// DMA Transfer and Start
#define A_DMA 0xFF46

// BG Palette
#define A_BGP 0xFF47
// 6-7  [11]bpp color
// 4-5  [10]bpp color
// 2-3  [01]bpp color
// 1-2  [00]bpp color
// Object Palettes
#define A_OBP0 0xFF48
#define A_OBP1 0xFF49

// Window X / Y
#define A_WY 0xFF4A
#define A_WX 0xFF4B
 
namespace GBC
{
  struct PixelRow
  {
    uint8_t bpp[8];
  };
  
  struct Tile
  {
    PixelRow row[8];
  };

  struct FIFO
  {
    uint8_t bpp;

    uint8_t palette;

    uint8_t bg_prio;
  };
  
  struct Line
  {
    FIFO fif[WIDTH];
  };
  
  struct Screen
  {
    Line line[HEIGHT];
  };

  struct Object
  {
    uint8_t y;
    uint8_t x;

    uint8_t index;
    uint8_t flags;

    uint8_t height;
  };

  struct Fetcher
  {
    uint8_t Push(uint8_t rend_x);

    void Fetch();
    void Discard();
    void Reset();
    
    Tile IndexToTile(uint8_t index, bool BGW, bool hflip = false, bool vflip = false);

    Object OAMToObject(uint8_t index);
    uint8_t TileToScreen(uint8_t x, uint8_t y, bool map2);

  private:
    enum class Mode
    {
      READ_TILE=0, READ_DATA0, READ_DATA1, PUSH_FIFO, NONE
    };

    enum class TileMode
    {
      BG=0, W, OBJ, NONE
    };
    
    Mode state = Mode::READ_TILE;
    TileMode tile_mode = TileMode::NONE;
    
    void Read_Tile();

    void Read_Data0();
    void Read_Data1();
    
    void Push_FIFO();

    void Pop(bool obj);

    bool start = true;
    bool scx_done = false;
    
    uint8_t x = 0;
    uint8_t y = 0;

    uint8_t w_x = 0;
    uint8_t w_y = 0;

    FIFO fetch[8];
    FIFO fetch_obj[8];

    FIFO fifo_bg[8];
    uint8_t bg_size = 0;

    FIFO fifo_obj[8];
    uint8_t obj_size = 0;

    Object current_obj;

    // 0 : x == x
    // 1 : begin
    // 2 : end
    uint8_t beg = 0;

    Object next_obj;
    bool cobj = false;
  public:
    FIFO sprite_line[WIDTH];

    uint8_t window_line_counter = 0;
    bool window_trigger = false;
    bool line_begin = true;
    bool window_begin = true;
    
    Object buffer[40];
    uint8_t sprite_size = 0;
    
    uint8_t *LCDC;

    uint8_t *SCX;
    uint8_t *SCY;
    
    uint8_t *WX;
    uint8_t *WY;

    uint8_t *LY;

    Screen *screen;

    uint8_t *tile_data;
    
    uint8_t *map1;
    uint8_t *map2;
    uint8_t *oam;

    uint8_t *OBP0;
    uint8_t *OBP1;
    uint8_t *BGP;
  };

 
  struct PPU
  {
    // 15-Bit RGB Color
    PPU(Interrupt *interrupt);

    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

    void UpdateSettings();

    void UpdateMaps();
    void UpdateTiles();

    void UpdateOAM();
    void DMATransfer(uint8_t *chunk);
    
    void Render();
    void Tick();
 
  private:
    Interrupt *interrupt;

    enum class Mode
    {
      HBLANK=0, VBLANK, OAM_SCAN, DRAWING_PIXELS
    };

    struct Renderer
    {
      Mode mode = Mode::OAM_SCAN;
      uint16_t dot = 0;

      uint8_t x = 0;
    };

    Renderer rend;
    Fetcher fetch;

    uint8_t tile_data[0x1800] = {0};
    uint8_t map1[32*32] = {0};
    uint8_t map2[32*32] = {0};

    uint8_t oam[0xA0] = {0};
    Object objects[40] = {0};
    
    Object pre_buffer[40] = {0};
    uint8_t pre_buffer_size = 0;
    void Draw_Pixel();
  public:
    // for debug only
    int frames = 0;
    bool frame_done = false;
    
    Tile tile[384*2] = {0};
    Tile tmap1[32*32] = {0};
    Tile tmap2[32*32] = {0};

    Tile OAM_tiles[40] = {0};

    Screen screen;
  private:
    bool line_interrupt_done = false;
    uint8_t LCDC = 0;
    uint8_t STAT = 0;

    uint8_t SCX = 0;
    uint8_t SCY = 0;
    
    uint8_t WX = 0;
    uint8_t WY = 0;

    uint8_t LY = 0;
    uint8_t LYC = 0;

    uint8_t DMA = 0;
    
    uint8_t BGP = 0;

    uint8_t OBP0 = 0;
    uint8_t OBP1 = 0;
  };
};
