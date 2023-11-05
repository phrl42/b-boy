#include "gbc/bus.h"
#include "Application.hpp"

namespace GBC
{
  void Bus::Emulate_Cycle(uint8_t n, bool normal)
  {
    uint8_t cpu_speed = NORMAL_SPEED;
    if(!normal)
    {
      cpu_speed = DOUBLE_SPEED;
    }

    double elapsed_time = Banana::Application::GetInstance().GetWindow().GetTime();
    while(elapsed_time < (double)((1 / (cpu_speed * 1000)) * n))
    {
      double begin = Banana::Application::GetInstance().GetWindow().GetTime();
      double end = Banana::Application::GetInstance().GetWindow().GetTime();
      elapsed_time += (double)((end - begin));
    }
  }

  uint8_t Bus::Read(uint16_t address)
  {
    // simple for now
    return space[address];
  }
  
  uint8_t Bus::Write(uint16_t address, uint8_t value)
  {
    // simple for now
    space[address] = value;

    return 1;
  }
  
};
