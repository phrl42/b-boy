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
      :rom(ROM()), interrupt(Interrupt()), ppu(PPU()), io(IO()), timer(Timer(&interrupt)), bus(Bus(&ppu, &timer, &interrupt, &io, &rom)), cpu(CPU(&bus))
    {

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

    // used to terminate thread
    bool kill = false;
    
    // used for imgui debug
    std::vector<std::pair<uint16_t, std::string>> instructions;

    int breakaddr = 0xFFFFF;
    bool breakfree = false;

    int breakop = -1;
    
    void add_address(uint16_t address, std::string mnemonic);
  };
 
};
