#include "gbc/interrupt.h"
#include "gbc/cpu.h"

namespace GBC
{
  Interrupt::Interrupt()
  {

  }

  void Interrupt::Request(INTERRUPT isr)
  {
    Set_Bit_N(&IF, isr, 1);
  }

  uint8_t Interrupt::Read(uint16_t address)
  {
    switch(address)
    {
    case A_IF:
      return IF;
      break;

    case A_IE:
      return IE;
      break;

    default:
      GBC_LOG("[INTERRUPT]: WRONG READ");
    }
    return 0;
  }

  void Interrupt::Write(uint16_t address, uint8_t value)
  {
    switch(address)
    {
    case A_IF:
      IF = value;
      break;

    case A_IE:
      IE = value;
      break;

    default:
      GBC_LOG("[INTERRUPT]: WRONG READ");
    }
  }
  
  void Interrupt::Handle_Type(INTERRUPT isr, uint16_t address)
  {
    printf("calling interrupt: %d\n", (int)isr);
    cpu->PUSH(&cpu->PC, IMode::NONE, nullptr, IMode::NONE);

    cpu->PC = address;
  }

  bool Interrupt::Check_Type(INTERRUPT isr, uint16_t address)
  {
    if(Get_Bit_N(IF, isr) && Get_Bit_N(IE, isr))
    {
      Handle_Type(isr, address);

      Set_Bit_N(&IF, isr, 0);
      cpu->IME = false;
      return true;
    }

    return false;
  }
 
  
  void Interrupt::Handle(CPU *cpu)
  {
    this->cpu = cpu;
    
    if(Check_Type(INTERRUPT::VBLANK, 0x40))
    {

    }
    else if(Check_Type(INTERRUPT::LCD, 0x48))
    {

    }
    else if(Check_Type(INTERRUPT::TIMER, 0x50))
    {

    }
    else if(Check_Type(INTERRUPT::SERIAL, 0x58))
    {

    }
    else if(Check_Type(INTERRUPT::JOYPAD, 0x60))
    {

    }
  }
};
