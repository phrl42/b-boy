#include <cstring>
#include <sstream>
#include <cstdlib>

#include "gbc/gbc.h"
#include "gbc/bitwise.h"
#include "gbc/cpu.h"

namespace GBC 
{
  bool Spec::Load_Rom(const char* rom_path)
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
      
    // put all 32 KiB into ram
    char byte = 0;
    uint16_t index = entry;
    while(file.get(byte))
    {
      bus.Write(index, byte);
      index += 1;
    }
    GBC_LOG("Loaded " + std::to_string(index - entry) + " bytes");

    file.close();
    return true;
  }

  
  void Spec::Init(const char* rom_path)
  {
    // zero all arrays
    for(int y = 0; y < TFT_HEIGHT; y++)
    {
      for(int x = 0; x < TFT_WIDTH; x++)
      {
	ppu.display[y][x] = 0;
      }
    }

    if(!Load_Rom(rom_path))
    {
      GBC_LOG("Could not load ROM at: " + std::string(rom_path));
    }
    else
    {
      GBC_LOG("Loaded ROM '" + std::string(rom_path) + "'");
    }

    cpu.state = State::RUN;
    cpu.PC = entry; // set Program Counter to entry
    rom = rom_path;
  }

  void Spec::Update()
  {
    if(cpu.state == State::RUN)
    {
      cpu.Validate_Opcode(&bus);
    }
  }
  
};
