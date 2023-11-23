#pragma once
#include "Sandbox.h"

#include "gbc/ppu.h"
#include "gbc/bus.h"
#include "gbc/cpu.h"
#include "gbc/interrupt.h"

namespace GBC
{
  const uint16_t entry = 0x0100;
 
  struct Spec
  {
    inline Spec()
    :cpu(CPU(&bus)), ppu(PPU(&bus)), interrupt(Interrupt(&bus, &cpu))
    {

    }
    
    Bus bus;
    CPU cpu;
    PPU ppu;

    Interrupt interrupt;
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
