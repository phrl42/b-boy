#pragma once
#include "Sandbox.h"

#define A_ENABLE_RAM 0x0000
#define A_ENABLE_RAM_END 0x1FFF

#include <cstdint>

namespace GBC
{
  const uint16_t entry = 0x0100;

  struct ROM
  {
    inline ROM(){}
    ~ROM();
    
    enum class MBC
      {
	NONE=0, MBC1, MBC1B
      };
    bool Load_Rom(const char* rom_path);
    void Init(const char* rom_path);

    uint8_t Read(uint16_t address);

    void Write(uint16_t address, uint8_t value);

    void Post_Bios();

    inline const char* Get_Rom_Name() { return rom; }
  private:
    MBC mbc;
    const char* rom;
    uint8_t space[32768 * 64];

    uint8_t lower_game[0x0100];

    uint8_t ROM_BANK_NUM = 0;
    uint8_t RAM_BANK_NUM = 0;

    uint8_t mode = 0;

    bool external_ram = false;

    uint32_t rom_size = 0;
    uint8_t banks = 0;

    uint8_t mask = 0;
  };
};
