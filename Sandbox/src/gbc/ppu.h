#pragma once
#include "Sandbox.h"
#include "gbc/bitwise.h"

namespace GBC
{
  struct PPU
  {
    // 15-Bit RGB Color
    //uint16_t display[TFT_HEIGHT][TFT_WIDTH];
    
  private:
    // PPU Registers are located in RAM

    // Object Attribute Map
    uint16_t OAM = 0xFE00;
    // 4 byte space 
    
    // LCD Control (R/W)
    uint16_t LCDC = 0xFF40;
      // 7 LCD Display Enable
      // 6 Window Tile Map Address
      // 5 Window Enable
      // 4 BG & Window Tile Data
      // 3 BG Tile Map Address
      // 2 OBJ Size
      // 1 OBJ Enable
      // 0 BG Enable
    
    // LCD Status (R/W)
    uint16_t STAT = 0xFF41;
      // 6 LYC=LY Interrupt
      // 5 Mode 2 OAM Interrupt
      // 4 Mode 1 V-Blank Interrupt
      // 3 Mode 0 H-Blank Interrupt
      // 2 LYC=LY Flag
      // 1-0 Mode 
         
    // Scroll X / Y
    uint16_t SCY = 0xFF42;
    uint16_t SCX = 0xFF43;

    // LCDC Y Coordinate
    uint16_t LY = 0xFF44;
    // LY Compare
    uint16_t LYC = 0xFF45;

    // DMA Transfer and Start
    uint16_t DMA = 0xFF46;

    // BG Palette
    uint16_t BGP = 0xFF47;
      // 6-7  [11]bpp color
      // 4-5  [10]bpp color
      // 2-3  [01]bpp color
      // 1-2  [00]bpp color
    // Object Palettes
    uint16_t OBP0 = 0xFF48;
    uint16_t OBP1 = 0xFF49;

    // Window X / Y
    uint16_t WY = 0xFF4A;
    uint16_t WX = 0xFF4B;
  };
};
