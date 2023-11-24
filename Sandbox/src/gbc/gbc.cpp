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
      
    // put game rom into ram starting at 0x0100
    // put bootrom before
    
    char byte = 0;
    uint16_t index = 0;

    //std::ifstream bootrom("assets/roms/bootrom.gb");
    //while(bootrom.get(byte))
    //{
    //bus.Write(index, byte);
    //index += 1;
    //}

    //if(index != entry) GBC_LOG("Loading BootRom failed.");

    index = 0x0;
    //index = entry;
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
	//ppu.display[y][x] = 0;
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

    cpu.state = State::HALT;
    rom = rom_path;
  }

  void Spec::Serial_Update()
  {
    if(bus.Read(0xFF02) == 0x81)
    {
      char c = bus.Read(0xFF01);
      serial[serial_size++] = c;

      bus.Write(0xFF02, 0);
    }
  }

  void Spec::Serial_Print()
  {
    if(serial[0])
    {
      printf("[SERIAL]: %s\n", serial);
    }
  }
  
  void Spec::Update()
  {
    if(cpu.state == State::RUN)
    {
      cpu.Validate_Opcode();
      ppu.Render();
    }

    if(cpu.state == State::HALT)
    {
      if(bus.Read(IF))
      {
	cpu.state = State::RUN;
      }
    }
    
    if(cpu.IME)
    {
      interrupt.Handle();
    }

    Serial_Update();
    Serial_Print();
  }
  
};
