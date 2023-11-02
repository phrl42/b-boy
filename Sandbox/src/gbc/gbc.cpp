#include <cstring>
#include <sstream>
#include <cstdlib>

#include "gbc/gbc.h"
#include "gbc/bitwise.h"
#include "gbc/cpu.h"

namespace GBC 
{
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
      
    // put all 32 KiB into ram
    char byte = 0;
    uint16_t index = entry;
    while(file.get(byte))
    {
      spec->ram[index] = byte;
      index += 1;
    }
    GBC_LOG("Loaded " + std::to_string(index - entry) + " bytes");

    file.close();
    return true;
  }

  
  void Init_Spec(Spec *spec, const char* rom_path)
  {
    // zero all arrays
    for(int y = 0; y < TFT_HEIGHT; y++)
    {
      for(int x = 0; x < TFT_WIDTH; x++)
      {
	spec->gpu.display[y][x] = 0;
      }
    }

    if(!Load_Rom(spec, rom_path))
    {
      GBC_LOG("Could not load ROM at: " + std::string(rom_path));
    }
    else
    {
      GBC_LOG("Loaded ROM '" + std::string(rom_path) + "'");
    }

    spec->state = State::RUN;
    spec->PC = entry; // set Program Counter to entry
    spec->rom = rom_path;
  }

  void Update(Spec *spec)
  {
    if(spec->state == State::RUN)
    {
      Validate_Opcode(spec);
    }

    for(int y = 0; y < TFT_HEIGHT; y++)
    {
      for(int x = 0; x < TFT_WIDTH; x++)
      {
	if(spec->gpu.display[y][x])
	{
	  GBC_LOG("well shit");
	}
      }
    }

  }
  
};
