#pragma once
#include "Sandbox.h"

#include "gbc/ppu.h"
#include "gbc/bus.h"
#include "gbc/cpu.h"
#include "gbc/interrupt.h"
#include "gbc/timer.h"

namespace GBC
{
  const uint16_t entry = 0x0100;
 
  struct Spec
  {
    inline Spec()
    :ppu(PPU()), timer(Timer()), interrupt(Interrupt()), bus(Bus(&ppu, &timer, &interrupt)), cpu(CPU(&bus))
    {

    }
    
    PPU ppu;
    Timer timer;
    Interrupt interrupt;

    Bus bus;

    CPU cpu;

    const char* rom;

    void Init(const char* rom_path);
    void Update();

  private:
    bool Load_Rom(const char* rom_path);

    char serial[1024] = {0};
    int serial_size = 0;
    
    void Serial_Update();
    void Serial_Print();
  };
 
};
