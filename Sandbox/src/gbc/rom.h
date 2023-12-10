#pragma once
#include "Sandbox.h"

#include <cstdint>

namespace GBC
{
  const uint16_t entry = 0x0100;

  struct ROM
  {
    bool Load_Rom(const char* rom_path);
    void Init(const char* rom_path);

    uint8_t Read(uint16_t address);

    void Write(uint16_t address, uint8_t value);

    void Post_Bios();

    inline const char* Get_Rom_Name() { return rom; }
  private:
    const char* rom;
    uint8_t space[32768 * 100];

    uint8_t lower_game[0x0100];
  };
};
