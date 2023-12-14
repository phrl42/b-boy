#include "gbc/rom.h"
#include "gbc/bitwise.h"

namespace GBC
{
  bool ROM::Load_Rom(const char* rom_path)
  {
    std::ifstream file;
    file.open(rom_path, std::ios::binary | std::ios::in);
    
    if(!file)
    {
      return false;
    }

    // get file length
    int length = 0;
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
      
    char byte = 0;
    uint32_t index = 0;

    while(file.get(byte))
    {
      if(index < entry)
      {
	lower_game[index] = byte;
      }
      else
      {
	space[index] = byte;
      }

      index += 1;
    }

    GBC_LOG("Loaded " + std::to_string(index) + " bytes");

    rom_size = index;
    banks = (rom_size / 1024) / 16;
    switch(banks)
    {
    case 2:
    {
      mask = 0b00000001;
      break;
    }

    case 4:
    {
      mask = 0b00000011;
      break;
    }

    case 8:
    {
      mask = 0b00000111;
      break;
    }

    case 16:
    {
      mask = 0b00001111;
      break;
    }

    case 32:
    {
      mask = 0b00011111;
      break;
    }

    case 64:
    {
      mask = 0b00011111;
      break;
    }

    case 128:
    {
      mask = 0b00011111;
      break;
    }

    default:
    {
      GBC_LOG("[ROM] rom_size error when setting rom bank");
      break;
    }
    }

    index = 0;
    std::ifstream bootrom;
    bootrom.open("assets/roms/bootrom.gb", std::ios::binary | std::ios::in);
    
    while(bootrom.get(byte))
    {
      space[index] = byte;
      index += 1;
    }

    if(index != entry) GBC_LOG("Loading BootRom failed.");

    file.close();
    return true;
  }
  
  void ROM::Init(const char* rom_path)
  {
    if(!Load_Rom(rom_path))
    {
      GBC_LOG("Could not load ROM at: " + std::string(rom_path));
    }
    else
    {
      GBC_LOG("Loaded ROM '" + std::string(rom_path) + "'");
    }

    rom = rom_path;
  }

  void ROM::Post_Bios()
  {
    for(uint16_t i = 0; i < entry; i++)
    {
      space[i] = lower_game[i];
    }
  }

  ROM::~ROM()
  {
    if(mbc != MBC::MBC1B) return;
    std::ofstream save;
    save.open(std::string(rom) + std::string(".sav"), std::ios::binary | std::ios::out);

    for(uint16_t i = 0xA000; i <= 0xBFFF; i++)
    {
      save.put(space[i]);
    }

    save.close();
  }

  uint8_t ROM::Read(uint16_t address)
  {
    if(banks == 2) return space[address];
    if(address >= 0x0000 && address <= 0x3FFF)
    {
      if(mode == 0)
      {
	return space[address];
      }
    }

    if(address >= 0x4000 && address <= 0x7FFF)
    {
      uint16_t HIGH_BANK_NUM = 0;

      if(banks <= 32)
      {
	HIGH_BANK_NUM = ROM_BANK_NUM & mask;
      }
      return space[0x4000 * HIGH_BANK_NUM + (address - 0x4000)];
    }
  }

  void ROM::Write(uint16_t address, uint8_t value)
  {
    if(address <= 0x1FFF && address >= 0x0000)
    {
      if((value & 0x0F) == 0xA)
      {
	external_ram = true;
      }
      else
      {
	external_ram = false;
      }
    }

    if(address >= 0x2000 && address <= 0x3FFF)
    {
      if(value == 0) ROM_BANK_NUM = 1;
      
      value = value & mask;

      ROM_BANK_NUM = value;
    }

    if(address >= 0x4000 && address <= 0x5FFF)
    {
      RAM_BANK_NUM = value & 0x03;
    }

    if(address >= 0x6000 && address <= 0x7FFF)
    {
      mode = value & 0x01;
    }

    if(address >= 0xA000 && address <= 0xBFFF)
    {
      if(external_ram) space[address] = value;
    }
  }
};
