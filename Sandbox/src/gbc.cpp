#include "gbc.h"
#include <cstring>
#include <sstream>
#include <cstdlib>

namespace GBC 
{
  uint16_t Character_Address(uint8_t ch)
  {
    return ch * 5;
  }
  
  bool Load_Rom(Spec *spec, const char* rom_path)
  {
    std::ifstream file(rom_path);
    
    if(!file)
    {
      return false;
    }

    // get file length
    int length = 0;
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
      
    // put all 32 KiB KiB into ram
    char byte = 0;
    uint16_t index = 0;
    while(file.get(byte))
    {
      spec->ram[index] = byte;
      index += 1;
    }
    GBC_LOG("Loaded " + std::to_string(length) + " bytes");

    file.close();
    return true;
  }

  uint16_t Combine(uint8_t first, uint8_t second)
  {
    uint16_t first_16 = (uint16_t) first; 
    uint16_t second_16 = (uint16_t) second;

    first_16 = first_16 << 8;

    uint16_t val = first_16 | second_16;

    return val;
  }

  uint16_t Get_Value_N(uint16_t opcode, uint8_t n)
  {
    if(n > 3)
    {
      GBC_LOG("Cannot get " + std::to_string(n) + ". Opcode is 16-bit only: 0-3 MAX");
      return 0;
    }
    opcode = opcode << (n * 4);
    opcode = opcode >> 12;

    return opcode;
  }
  
  void Init_Spec(Spec *spec, const char* rom_path)
  {
    // zero all arrays
    memset(spec->ram, 0, sizeof(spec->ram) / sizeof(spec->ram[0]));

    for(int y = 0; y < TFT_HEIGHT; y++)
    {
      for(int x = 0; x < TFT_WIDTH; x++)
      {
	spec->display[y][x] = 0;
      }
    }

    if(!Load_Rom(spec, rom_path))
    {
      GBC_LOG("Could not load ROM at: " + std::string(rom_path));
    }
    
    GBC_LOG("Loaded ROM '" + std::string(rom_path) + "'");
    
    spec->state = State::RUN;
    spec->PC = 0; // set Program Counter to entry
    spec->rom = rom_path;
  }

  void Validate_Opcode(Spec *spec)
  {
    uint16_t opcode = Combine(spec->ram[spec->PC], spec->ram[spec->PC+1]);
    uint16_t prefix = opcode >> 12;

    spec->PC += 2;
  }
  
  void Update(Spec *spec)
  {
    Validate_Opcode(spec);
  }
  
};
