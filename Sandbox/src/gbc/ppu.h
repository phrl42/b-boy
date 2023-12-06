#pragma once
#include "Sandbox.h"
#include "gbc/bitwise.h"

#include <queue>

#define WIDTH 160
#define HEIGHT 144

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

  struct Line
  {
    uint8_t bpp[WIDTH];
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
 
  struct PPU
  {
    // 15-Bit RGB Color
    PPU();

    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

    void UpdateSettings();

    void UpdateMaps();
    void UpdateTiles();

    void UpdateOAM();
    void DMATransfer(uint8_t *chunk);
    
    Tile IndexToTile(uint8_t index, bool BGW);
    Object OAMToObject(uint8_t index);

    void Render();
    void Tick();

    uint8_t TileToScreen(uint16_t x, uint16_t y, bool map2);
    
  private:

    enum class Mode
    {
      ZERO=0, ONE, TWO, THREE
    };

    struct FIFO
    {
      uint8_t bpp = 0;
      uint8_t obp = 0;

      // 7th bit of attribute flag
      uint8_t priority = 0;
    };

    // always fetch a map[index] row
    void Step1();
    void Step2();
    void Step3();
    void Step4();
    void Push();
    
    struct Pixel_Fetcher
    {
      std::queue<FIFO> fifo_bg;
      std::queue<FIFO> fifo_obj;

      uint8_t current_step = 1;

      uint8_t skip = 0;
      
      uint8_t x = 0;
      uint8_t y = 0;
    };

    struct Renderer
    {
      Mode mode = Mode::TWO;
      uint16_t dot = 0;

      uint8_t x = 0;

      Object buffer[10] = {0};
      uint8_t buffer_index = 0;

      Pixel_Fetcher pixfetcher;
    };

    Renderer rend;

    uint8_t tile_data[0x1800] = {0};
    uint8_t map1[32*32] = {0};
    uint8_t map2[32*32] = {0};

    uint8_t oam[0xA0] = {0};
    Object objects[40] = {0};

  public:
    // for debug only
    Tile tile[384*2] = {0};
    Tile tmap1[32*32] = {0};
    Tile tmap2[32*32] = {0};

    Tile OAM_tiles[40] = {0};

    Screen screen;
  private:
    // registers
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
