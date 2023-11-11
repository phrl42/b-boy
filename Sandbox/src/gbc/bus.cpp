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

    // todo
    /*while(dt < (double)((1 / (cpu_speed * 1000000)) * n))
    {
      double b = Banana::Application::GetInstance().GetWindow().GetTime();
      double e = Banana::Application::GetInstance().GetWindow().GetTime();
      dt += (double)((e - b));
      std::cout << "testing calculation" << std::endl;
    }

    begin = Banana::Application::GetInstance().GetWindow().GetTime();
    */
  }

  uint8_t Bus::Read(uint16_t address)
  {
    return space[address];
  }
  
  uint8_t Bus::Write(uint16_t address, uint8_t value)
  {
    space[address] = value;
    return 0;
  }
  
};
