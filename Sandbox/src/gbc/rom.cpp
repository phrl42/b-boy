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
    uint16_t index = 0;

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

  uint8_t ROM::Read(uint16_t address)
  {
    if(!(address <= 0x7FFF))
    {
      GBC_LOG("[ROM] Wrong reading");
      return 0;
    }
    return space[address];
  }

  void ROM::Write(uint16_t address, uint8_t value)
  {
    if(!(address <= 0x7FFF))
    {
      GBC_LOG("[ROM] Wrong writing");
      return;
    }

    //space[address] = value;
    return;
  }
};
