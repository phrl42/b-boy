#pragma once
#include "Sandbox.h"

#include "gbc/ppu.h"
#include "gbc/bus.h"
#include "gbc/cpu.h"

namespace GBC
{
  const uint16_t entry = 0;
 
  struct Spec
  {
    inline Spec()
      :cpu(CPU(&bus))
    {

    }
    
    Bus bus;
    CPU cpu;
    PPU ppu;

    const char* rom;

    void Init(const char* rom_path);
    void Update();

  private:
    bool Load_Rom(const char* rom_path);
  };
 
};
