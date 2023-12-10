#pragma once
#include "Sandbox.h"
#include <vector>

#include "gbc/ppu.h"
#include "gbc/bus.h"
#include "gbc/cpu.h"
#include "gbc/interrupt.h"
#include "gbc/timer.h"
#include "gbc/io.h"
#include "gbc/rom.h"

namespace GBC
{
  enum class Debug
  {
    RUN, STOP, STEP
  };
  
  struct Spec
  {
    inline Spec()
      :rom(ROM()), interrupt(Interrupt()), ppu(PPU(&interrupt)), io(IO(&interrupt)), timer(Timer(&interrupt)), bus(Bus(&ppu, &timer, &interrupt, &io, &rom)), cpu(CPU(&bus))
    {
      for(int i = 0; i < 0x10000; i++)
      {
	instructions.push_back(std::pair<uint16_t, std::string>(0, "NONE"));
      }
    }
    
    PPU ppu;
    Timer timer;
    Interrupt interrupt;
    IO io;
    ROM rom;
    Bus bus;
    CPU cpu;

    Debug dstate = Debug::RUN;
    
    void Init(const char* rom_path);
    void Update();

    // used for imgui debug
    void add_address(int &address);
    std::vector<std::pair<uint16_t, std::string>> instructions;

    int breakaddr = 0xFFFFF;
    bool breakfree = false;

    int breakop = -1;
    
    void add_address(uint16_t address, std::string mnemonic);
  };
 
};
