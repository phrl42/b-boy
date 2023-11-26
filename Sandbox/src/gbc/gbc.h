#pragma once
#include "Sandbox.h"

#include "gbc/ppu.h"
#include "gbc/bus.h"
#include "gbc/cpu.h"
#include "gbc/interrupt.h"
#include "gbc/timer.h"
#include "gbc/io.h"

namespace GBC
{
  const uint16_t entry = 0x0100;
 
  struct Spec
  {
    inline Spec()
    :interrupt(Interrupt()), ppu(PPU()), io(IO()), timer(Timer(&interrupt)), bus(Bus(&ppu, &timer, &interrupt, &io)), cpu(CPU(&bus))
    {

    }
    
    PPU ppu;
    Timer timer;
    Interrupt interrupt;
    IO io;
    
    Bus bus;

    CPU cpu;

    const char* rom;

    void Init(const char* rom_path);
    void Update();

    // used to terminate thread
    bool kill = false;
  private:
    bool Load_Rom(const char* rom_path);

  };
 
};
