#pragma once
#include "Sandbox.h"
#include "gbc/bitwise.h"

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
// 4 byte space 
    
// LCD Control (R/W)
#define A_LCDC 0xFF40
// 7 LCD Display Enable
// 6 Window Tile Map Address
// 5 Window Enable
// 4 BG & Window Tile Data
// 3 BG Tile Map Address
// 2 OBJ Size
// 1 OBJ Enable
// 0 BG Enable
    
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
    // saved with 2bpp
    PixelRow row[8];
  };
  
  struct PPU
  {
    // 15-Bit RGB Color
    PPU();

    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);
    
    void Render();

    void UpdateMaps();
    
    Tile IndexToTile(uint8_t index, bool BGW);
    
  private:
    uint8_t tile_data[A_TileDataEND - A_TileData] = {0};

    uint8_t map1[32*32] = {0};
    uint8_t map2[32*32] = {0};

  public:
    Tile tmap1[32*32] = {0};
    Tile tmap2[32*32] = {0};

  private:
    uint8_t LCDC = 16;
    uint8_t STAT = 0;

    uint8_t SCX = 0;
    uint8_t SCY = 0;

    uint8_t LY = 0;
    uint8_t LYC = 0;

    uint8_t DMA = 0;
    
    uint8_t BGP = 0;
  };
};
