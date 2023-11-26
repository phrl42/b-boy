#include "gbc/timer.h"

namespace GBC
{
  Timer::Timer(Interrupt *interrupt)
  {
    this->interrupt = interrupt;
    DIV = 0xABCC;
  }

  uint8_t Timer::Read(uint16_t address)
  {
    switch(address)
    {
    case A_DIV:
      return DIV >> 8;
      break;

    case A_TIMA:
      return TIMA;
      break;

    case A_TMA:
      return TMA;
      break;

    case A_TAC:
      return TAC;
      break;

    default:
      GBC_LOG("[Timer]: Invalid Read");
      break;
    }
  }

  void Timer::Write(uint16_t address, uint8_t value)
  {
    switch(address)
    {
    case A_DIV:
      DIV = 0;
      break;

    case A_TIMA:
      TIMA = value;
      break;

    case A_TMA:
      TMA = value;
      break;

    case A_TAC:
      TAC = value;
      break;

    default:
      GBC_LOG("[Timer]: Invalid Write");
      break;
    }
  }

  void Timer::Tick()
  {
    uint16_t prev_div = DIV;

    DIV++;

    bool timer_update = false;
    
    switch(TAC & 0x11)
    {
    case 0x00:
      timer_update = (Get_Bit_N(prev_div, 9) && !Get_Bit_N(DIV, 9));
      break;

    case 0x01:
      timer_update = (Get_Bit_N(prev_div, 3) && !Get_Bit_N(DIV, 3));
      break;

    case 0x10:
      timer_update = (Get_Bit_N(prev_div, 5) && !Get_Bit_N(DIV, 5));
      break;

    case 0x11:
      timer_update = (Get_Bit_N(prev_div, 7) && !Get_Bit_N(DIV, 7));
      break;
    }

    if(timer_update && Get_Bit_N(TAC, 2))
    {
      TIMA++;

      if(TIMA == 0xFF)
      {
	TIMA = TMA;
	interrupt->Request(INTERRUPT::TIMER);
      }
      
    }
  }
};
