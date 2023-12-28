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

    enum class SIZE
    {
      NONE=0, KB2, KB8, KB32, KB64, KB128, KB256, KB512, MB1, MB2, MB4, MB8, MB1_1, MB1_2, MB1_5 
    };

    bool Load_Rom(const char* rom_path);
    void Init(const char* rom_path);

    uint8_t Read(uint16_t address);

    void Write(uint16_t address, uint8_t value);

    void Post_Bios();
    void Load_Save();

    inline const char* Get_Rom_Name() { return srom_path; }
  private:
    bool post_bios = false;
    bool loaded_save = false;
    MBC mbc = MBC::NONE;

    SIZE ram = SIZE::NONE;
    uint32_t ram_size_bytes = 0;

    SIZE rom = SIZE::NONE;
    uint32_t rom_size_bytes = 0;
    
    bool external_ram = false;
    
    const char* srom_path;
    uint8_t space[32768 * 64];
    uint8_t lower_game[0x0100];

    uint8_t mode = 0;
    uint8_t banks = 0;
    uint8_t mask = 0;

    uint8_t ROM_BANK_NUMBER = 0;
    uint8_t RAM_BANK_NUMBER = 0;

    uint8_t ZERO_BANK_NUMBER = 0;
    uint8_t HIGH_BANK_NUMBER = 0;
  };
};
