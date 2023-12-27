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

    uint8_t mbc_type = space[0x0147];

    switch(mbc_type)
    {
    case 0x00:
    {
      mbc = MBC::NONE;
      break;
    }

    case 0x01:
    {
      mbc = MBC::MBC1;
      break;
    }

    case 0x02:
    {
      mbc = MBC::MBC1;
      break;
    }

    case 0x03:
    {
      mbc = MBC::MBC1B;
      break;
    }

    default:
    {
      GBC_LOG("Unsupported MBC detected");
      break;
    }
    }
     
    uint8_t ram_size = space[0x0149];

    switch(ram_size)
    {
    case 0x00:
    {
      ram = SIZE::NONE;
      break;
    }

    case 0x01:
    {
      ram = SIZE::KB2;
      ram_size_bytes = 1024 * 2;
      break;
    }

    case 0x02:
    {
      ram = SIZE::KB8;
      ram_size_bytes = 1024 * 8;
      break;
    }

    case 0x03:
    {
      ram = SIZE::KB32;
      ram_size_bytes = 1024 * 32;
      break;
    }

    case 0x04:
    {
      ram = SIZE::KB128;
      ram_size_bytes = 1024 * 128;
      break;
    }

    case 0x05:
    {
      ram = SIZE::KB64;
      ram_size_bytes = 1024 * 64;
      break;
    }
    
    default:
    {
      GBC_LOG("Could not determine ram size");
      break;
    }
    }

    uint8_t rom_size = space[0x0148];

    ZERO_BANK_NUMBER = 0;
    HIGH_BANK_NUMBER = 0;
    
    switch(rom_size)
    {
    case 0x00:
    {
      rom = SIZE::KB32;
      mask = 0b00000001;
      rom_size_bytes = 1024 * 32;
      break;
    }

    case 0x01:
    {
      rom = SIZE::KB64;
      mask = 0b00000011;
      rom_size_bytes = 1024 * 64;
      break;
    }

    case 0x02:
    {
      rom = SIZE::KB128;
      mask = 0b00000111;
      rom_size_bytes = 1024 * 128;
      break;
    }

    case 0x03:
    {
      rom = SIZE::KB256;
      mask = 0b00001111;
      rom_size_bytes = 1024 * 256;
      break;
    }

    case 0x04:
    {
      rom = SIZE::KB512;
      mask = 0b00011111;
      rom_size_bytes = 1024 * 512;
      break;
    }

    case 0x05:
    {
      rom = SIZE::MB1;
      mask = 0b00011111;
      rom_size_bytes = 1024 * (1024);
      ZERO_BANK_NUMBER = (ROM_BANK_NUMBER & 0x01) << 5;
      break;
    }

    case 0x06:
    {
      rom = SIZE::MB2;
      mask = 0b00011111;
      rom_size_bytes = 1024 * (1024 * 2);
      ZERO_BANK_NUMBER = (ROM_BANK_NUMBER & 0x03) << 5;
      break;
    }

    case 0x07:
    {
      rom = SIZE::MB4;
      break;
    }

    case 0x08:
    {
      rom = SIZE::MB8;
      break;
    }

    case 0x52:
    {
      rom = SIZE::MB1_1;
      break;
    }

    case 0x53:
    {
      rom = SIZE::MB1_2;
      break;
    }

    case 0x54:
    {
      rom = SIZE::MB1_5;
      break;
    }

    default:
    {
      GBC_LOG("Could not determine rom size");
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

    std::ifstream save;
    save.open(std::string(rom_path) + std::string(".sav"), std::ios::binary | std::ios::in);

    if(save)
    {
      GBC_LOG("Loaded save at " + std::string(rom_path) + std::string(".sav"));
      char byte = 0;
      for(uint16_t address = 0xA000; address <= 0xBFFF; address++)
      {
	save.get(byte);
	space[address] = byte;
      }
      
    }
    
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

    srom_path = rom_path;
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
    save.open(std::string(srom_path) + std::string(".sav"), std::ios::binary | std::ios::out);

    for(uint16_t i = 0xA000; i <= 0xBFFF; i++)
    {
      save.put(space[i]);
    }

    save.close();
    GBC_LOG("Saved game to " + std::string(srom_path) + std::string(".sav"));
  }

  uint8_t ROM::Read(uint16_t address)
  {
    //if(banks == 2) return space[address];
    if(address >= 0x0000 && address <= 0x3FFF)
    {
      if(mode == 0)
      {
	return space[address];
      }

      return space[0x4000 * ZERO_BANK_NUMBER + address];
    }

    if(address >= 0x4000 && address <= 0x7FFF)
    {
      return space[0x4000 * HIGH_BANK_NUMBER + (address - 0x4000)];
    }

    if(address >= 0xA000 && address <= 0xBFFF)
    {
      return space[address];
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
    }

    if(address >= 0x2000 && address <= 0x3FFF)
    {
      if(value == 0) ROM_BANK_NUMBER = 1;
      
      value = value & mask;

      ROM_BANK_NUMBER = value;
      
      ZERO_BANK_NUMBER = 0;
      if(rom == SIZE::MB1) ZERO_BANK_NUMBER = (ROM_BANK_NUMBER & 0x01) << 5; 
      if(rom == SIZE::MB2) ZERO_BANK_NUMBER = (ROM_BANK_NUMBER & 0x03) << 5;
    }

    if(address >= 0x4000 && address <= 0x5FFF)
    {
      RAM_BANK_NUMBER = value & 0x03;

      HIGH_BANK_NUMBER = ROM_BANK_NUMBER & mask;
      if(rom == SIZE::MB1) HIGH_BANK_NUMBER = (RAM_BANK_NUMBER & 0x01) << 5;
      if(rom == SIZE::MB2) HIGH_BANK_NUMBER = (RAM_BANK_NUMBER & 0x03) << 5;
    }

    if(address >= 0x6000 && address <= 0x7FFF)
    {
      mode = value & 0x01;
    }

    if(address >= 0xA000 && address <= 0xBFFF)
    {
      if(!external_ram) return;
      uint16_t addr = address;

      if(ram == SIZE::KB2 || ram == SIZE::KB8) addr = ((address - 0xA000) % rom_size_bytes);
      if(mode == 1 && ram == SIZE::KB32) addr = 0x2000 * RAM_BANK_NUMBER + (address - 0xA000);
      if(mode == 0) addr = address - 0xA000;
      
      space[addr] = value;
    }
  }
};
