#include "gbc/cpu.h"
#include "gbc/bitwise.h"
#include "gbc/interrupt.h"

#include "Application.hpp"

#define Z_FLAG 7
#define N_FLAG 6
#define H_FLAG 5
#define C_FLAG 4

namespace GBC
{

  CPU::CPU(Bus *bus)
    :bus(bus)
  {

  }
  
  void CPU::Validate_Opcode()
  {
    if(eIME)
    {
      eIME = false;
      IME = true;
    }

    // keep in mind: the GameBoy CPU (SM83) has Little-Endianness (reads multiple bytes backwards from ram)
    uint8_t opcode = bus->Read(PC);
    (this->*lookup[opcode].opfun)(lookup[opcode].dest, lookup[opcode].w, lookup[opcode].src, lookup[opcode].r);
    PC += 1;
  }

  void CPU::Set_Half_Carry_DEC(uint8_t src_register)
  {
    src_register -= 1;
    Set_Bit_N(&AF, H_FLAG, (src_register & 0x0F) == 0x0F);
    return;
  }

  void CPU::Set_Half_Carry_Minus(uint16_t src_register, int val)
  {
    bool h = ((int)(src_register & 0xF) - (int)(val & 0xF)) < 0;

    Set_Bit_N(&AF, H_FLAG, h);
    return;
  }
  
  void CPU::Set_Half_Carry_MinusC(uint16_t src_register, int val)
  {
    bool h = (int)(((int)(src_register & 0xF) - (int)(val & 0xF)) - (int)Get_Bit_N(AF, C_FLAG)) < 0;

    Set_Bit_N(&AF, H_FLAG, h);
    return;
  }

  void CPU::Set_Half_Carry(uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t erase_val = 0x0F;
    uint16_t check_val = 0x10;

    bool cval = 0;
    if(!bit8)
    {
      erase_val = 0x0FFF;
      check_val = 0x1000;
    }

    src_register &= erase_val;
    val &= erase_val;
    
    src_register += val;
    if(src_register >= check_val)
    {
      cval = 1;
    }      

    Set_Bit_N(&AF, H_FLAG, cval);
    return;
  }

  void CPU::Set_Half_CarryC(uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t erase_val = 0x0F;
    uint16_t check_val = 0x10;

    bool cval = 0;
    if(!bit8)
    {
      erase_val = 0x0FFF;
      check_val = 0x1000;
    }

    src_register &= erase_val;
    val &= erase_val;
    
    src_register += val;
    src_register += Get_Bit_N(AF, C_FLAG);
    if(src_register >= check_val)
    {
      cval = 1;
    }      

    Set_Bit_N(&AF, H_FLAG, cval);
    return;
  }

  void CPU::Set_Carry_Plus(uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t check_val = 0xFF;
    bool cval = 0;
    
    if(!bit8)
    {
      check_val = 0xFFFF;
    }

    if((int)(src_register + val) > check_val)
    {
      cval = 1;
    }

    Set_Bit_N(&AF, C_FLAG, cval);
    return;
  }

  void CPU::Set_Carry_PlusC(uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t check_val = 0xFF;
    
    bool cval = 0;
    
    if(!bit8)
    {
      check_val = 0xFFFF;
    }

    if((int)(((src_register + val) + Get_Bit_N(AF, C_FLAG))) > check_val)
    {
      cval = 1;
    }

    Set_Bit_N(&AF, C_FLAG, cval);
    return;
  }

  void CPU::Set_Carry_Minus(uint16_t src_register, uint8_t val)
  {
    uint8_t A = src_register;

    bool cval = 0;
    
    if((int)(A - val) < 0)
    {
      cval = 1;
    }

    Set_Bit_N(&AF, C_FLAG, cval);
    
    return;
  }
  
  void CPU::Set_Carry_MinusC(uint16_t src_register, uint8_t val)
  {
    uint8_t A = src_register;

    bool cval = 0;

    if((int)((int)A - (int)val - (int)Get_Bit_N(AF, C_FLAG)) < 0)
    {
      cval = 1;
    }

    Set_Bit_N(&AF, C_FLAG, cval);
    
    return;
  }

  uint8_t CPU::PREFIX(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    PC += 1;
    uint8_t opcode = bus->Read(PC);

    return (this->*lookup_cb[opcode].opfun)(lookup_cb[opcode].dest, lookup_cb[opcode].w, lookup_cb[opcode].src, lookup_cb[opcode].r);
  }

  uint8_t CPU::EI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    IME = true;

    return 4;
  }

  uint8_t CPU::DI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    IME = false;
    
    return 4;
  }

  uint8_t CPU::LD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;
    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = (uint8_t)*src_value;
    }

    if(r == IMode::ALL)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::MEMI)
    {
      val = bus->Read(*src_value);
      *src_value += 1;
    }

    if(r == IMode::MEMD)
    {
      val = bus->Read(*src_value);
      *src_value -= 1;
    }

    if(r == IMode::A16)
    {
      PC += 1;
      uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC)); 
      PC += 1;
      val = bus->Read(addr);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }
    
    if(r == IMode::N16)
    {
      PC += 1;
      val = Combine(bus->Read(PC+1), bus->Read(PC));
      PC += 1;
    }

    // special reading for 0xF8
    if(r == IMode::E8)
    {
      PC += 1;
      uint8_t val = bus->Read(PC);

      uint16_t uval = SP + (int8_t)val;
      
      Set_Bit_N(&AF, H_FLAG, (SP & 0x000F) + (val & 0x000F) >= 0x0010);
      Set_Bit_N(&AF, C_FLAG, (bool)((int)(((SP & 0x00FF) + (val & 0x00FF))) >= 0x0100));

      Set_Bit_N(&AF, N_FLAG, 0);
      Set_Bit_N(&AF, Z_FLAG, 0);
      *dest_register = uval;
      return 0;
    }
    
    // only counts for 0x08
    if(r == IMode::ALL && w == IMode::A16)
    {
      PC += 1;
      uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC)); 
      PC += 1;
      bus->Write(addr, val);
      bus->Write(addr+1, (val >> 8));
      return 0;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= val << 8;
    }

    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::ALL)
    {
      *dest_register = val;
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
    }

    if(w == IMode::MEMI)
    {
      bus->Write(*dest_register, val);
      *dest_register += 1;
    }

    if(w == IMode::MEMD)
    {
      bus->Write(*dest_register, val);
      *dest_register -= 1;
    }

    if(w == IMode::A16)
    {
      PC += 1;
      uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC));
      PC += 1;

      bus->Write(addr, val);
    }

    return 0;
  }


  // only covers instructions 0xE2 and 0xF2
  uint8_t CPU::LDS(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    // 0xE2
    if(w == IMode::LOW)
    {
      uint8_t val = *src_value >> 8;
      uint8_t addr = *dest_register;
      
      bus->Write(0xFF00 + addr, val);
      
    } 
    else // 0xF2
    {
      uint8_t val = bus->Read(0xFF00 + (uint8_t)(*src_value));
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    return 0;
  }
  
  uint8_t CPU::LDH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    // 0xE0
    if(w == IMode::A8)
    {
      PC += 1;
      uint8_t a8 = bus->Read(PC);
      bus->Write(0xFF00 + a8, *src_value >> 8);
    }
    else // 0xF0
    {
      PC += 1;
      uint8_t a8 = bus->Read(PC);
      uint8_t val = bus->Read(0xFF00 + a8);
      *dest_register &= 0x00FF;
      *dest_register |= val << 8;
    }
    return 0;
  }

  uint8_t CPU::INC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    if(w == IMode::ALL)
    {
      *dest_register += 1;
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, bus->Read(*dest_register)+1);
    }
   
    if(w == IMode::HIGH)
    {
      uint8_t H = *dest_register >> 8;
      Set_Half_Carry(H, 1, true);

      H += 1;

      *dest_register &= 0x00FF;
      *dest_register |=  H << 8;
      
      Set_Bit_N(&AF, Z_FLAG, (bool)(H == 0));
      Set_Bit_N(&AF, N_FLAG, 0);
    }

    if(w == IMode::LOW)
    {
      uint8_t L = *dest_register;
      Set_Half_Carry(L, 1, true);

      L +=1;

      *dest_register &= 0xFF00;
      *dest_register |= L;
      
      Set_Bit_N(&AF, Z_FLAG, (bool)(L == 0));
      Set_Bit_N(&AF, N_FLAG, 0);
    }

    return 0;
  }
  
  uint8_t CPU::DEC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    if(w == IMode::ALL)
    {
      *dest_register -= 0x01;
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, bus->Read(*dest_register)-1);
    }
    
    if(w == IMode::HIGH)
    {
      uint8_t H = *dest_register >> 8;
      
      Set_Half_Carry_DEC(H);

      H -= 1;
      *dest_register &= 0x00FF;
      *dest_register |= H << 8;
      
      Set_Bit_N(&AF, Z_FLAG, (bool)(H == 0));
      Set_Bit_N(&AF, N_FLAG, 1);
    }

    if(w == IMode::LOW)
    {
      uint8_t L = *dest_register;
      Set_Half_Carry_DEC(L);
      L -= 1;
      *dest_register &= 0xFF00;
      *dest_register |= L;
      
      Set_Bit_N(&AF, Z_FLAG, (bool)(L == 0));
      Set_Bit_N(&AF, N_FLAG, 1);
    }

    return 0;
  }

  uint8_t CPU::ADD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;
    Set_Bit_N(&AF, N_FLAG, 0);

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = (uint8_t)*src_value;
    }

    if(r == IMode::ALL)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    // 0xE8 special case
    if(r == IMode::E8)
    {
      PC += 1;
      uint8_t val = bus->Read(PC);

      Set_Bit_N(&AF, H_FLAG, (*dest_register & 0x000F) + (val & 0x000F) >= 0x0010);
      Set_Bit_N(&AF, C_FLAG, (bool)((int)(((*dest_register & 0xFF) + (val & 0xFF))) >= 0x0100));
      *dest_register += (int8_t)val;

      Set_Bit_N(&AF, Z_FLAG, 0);
      Set_Bit_N(&AF, N_FLAG, 0);
      return 0;
    }

    if(w == IMode::HIGH)
    {
      uint8_t H = *dest_register >> 8;
      
      Set_Half_Carry(H, val, true);
      Set_Carry_Plus(H, val, true);

      H += val;
      *dest_register &= 0x00FF;
      *dest_register |= H << 8;
      
      Set_Bit_N(&AF, Z_FLAG, (bool)(H == 0));
    }

    if(w == IMode::ALL)
    {
      Set_Half_Carry(*dest_register, val, false);
      Set_Carry_Plus(*dest_register, val, false);
      *dest_register += val;
    }

    return 0;
  }

  uint8_t CPU::SUB(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;

    Set_Bit_N(&AF, N_FLAG, 0);

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    if(w == IMode::HIGH)
    {
      uint8_t A = *dest_register >> 8;
      Set_Half_Carry_Minus(A, val);
      Set_Carry_Minus(A, val);
      
      A -= val;
      *dest_register &= 0x00FF;
      *dest_register |= A << 8;
      
      Set_Bit_N(&AF, Z_FLAG, (bool)(A == 0));
      Set_Bit_N(&AF, N_FLAG, 1);
    }

    return 0;
  }

  uint8_t CPU::AND(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::ALL)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    if(w == IMode::HIGH)
    {
      uint8_t A = *dest_register >> 8;

      A &= val;
      
      *dest_register &= 0x00FF;
      *dest_register |= A << 8;
    } 
     
    Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 1);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 0;
  }

  uint8_t CPU::OR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = (uint8_t)*src_value;
    }

    if(r == IMode::ALL)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    if(w == IMode::HIGH)
    {
      *dest_register |= val << 8;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 0;
  }

  uint8_t CPU::XOR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::ALL)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    if(w == IMode::HIGH)
    {
      uint8_t H = *dest_register >> 8;

      H ^= val;
      
      *dest_register &= 0x00FF;
      *dest_register |= H << 8;
      Set_Bit_N(&AF, Z_FLAG, (bool)(H == 0));
    }

    if(w == IMode::LOW)
    {
      uint8_t L = *dest_register;

      L ^= val;
      
      *dest_register &= 0xFF00;
      *dest_register |= L;

      Set_Bit_N(&AF, Z_FLAG, (bool)(L == 0));
    }

    if(w == IMode::ALL)
    {
      *dest_register ^= val;
      Set_Bit_N(&AF, Z_FLAG, (bool)(*dest_register == 0));
    }

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 0;
  }

  uint8_t CPU::POP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    // most delicious instruction
    if(bus->Read(PC) == 0xF1)
    {
      uint16_t reg_ram = 0;

      reg_ram |= (bus->Read(SP) & 0xF0);
      SP++;
      reg_ram |= bus->Read(SP) << 8;
      SP++;

      AF = reg_ram;

      return 0;
    }
    
    *dest_register &= 0x0000;
    *dest_register |= bus->Read(SP);
    SP++;
    *dest_register |= (bus->Read(SP) << 8);
    SP++;

    return 0;
  }

  uint8_t CPU::PUSH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    SP--;
    bus->Write(SP, (*dest_register >> 8));
    SP--;
    bus->Write(SP, *dest_register);

    return 0;
  }

  uint8_t CPU::JP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    PC += 2;

    if(w == IMode::Z)
    {
      if(!Get_Bit_N(AF, Z_FLAG)) return 0;
    }
    
    if(w == IMode::NZ)
    {
      if(Get_Bit_N(AF, Z_FLAG)) return 0;
    }

    if(w == IMode::C)
    {
      if(!Get_Bit_N(AF, C_FLAG)) return 0;
    }
   
    if(w == IMode::NC)
    {
      if(Get_Bit_N(AF, C_FLAG)) return 0;
    }

    if(w == IMode::ALL)
    {
      PC = *dest_register;
      PC -= 1;
      return 0;
    }

    PC -= 2;
      
    PC += 1;
    uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC));

    PC = addr;
    PC -= 1;
    
    return 4;
  }
  
  uint8_t CPU::JR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    PC += 1;
    
    if(w == IMode::Z)
    {
      if(!Get_Bit_N(AF, Z_FLAG)) return 0;
    }
    
    if(w == IMode::C)
    {
      if(!Get_Bit_N(AF, C_FLAG)) return 0;
    }

    if(w == IMode::NZ)
    {
      if(Get_Bit_N(AF, Z_FLAG)) return 0;
    }
    
    if(w == IMode::NC)
    {
      if(Get_Bit_N(AF, C_FLAG)) return 0;
    }

    if(r == IMode::E8 || w == IMode::E8)
    {
      PC -= 1;
      PC += (int8_t)bus->Read(PC+1);
      PC += 1;
    }

    return 4;
  }

  uint8_t CPU::CALL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    PC += 2;

    if(w == IMode::Z)
    {
      if(!Get_Bit_N(AF, Z_FLAG)) return 0;
    }
    if(w == IMode::NZ)
    {
      if(Get_Bit_N(AF, Z_FLAG)) return 0;
    }

    if(w == IMode::C)
    {
      if(!Get_Bit_N(AF, C_FLAG)) return 0;
    }
   
    if(w == IMode::NC)
    {
      if(Get_Bit_N(AF, C_FLAG)) return 0;
    }

    PC += 1;
    if(w == IMode::A16)
    {
      PC -= 2;
      uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC));

      PC += 2;
      SP--;
      bus->Write(SP, (PC >> 8));
      SP--;
      bus->Write(SP, PC);
      PC = addr;
    }

    if(r == IMode::A16)
    {
      PC -= 2;
      
      uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC));
      
      PC += 2;

      SP--;
      bus->Write(SP, (PC >> 8));
      SP--;
      bus->Write(SP, PC);
      PC = addr;
    }

    PC -= 1;
    return 12;
  }

  uint8_t CPU::RST(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t lowpage = 0;

    if(w == IMode::R10) lowpage = 0x10;
    if(w == IMode::R20) lowpage = 0x20;
    if(w == IMode::R30) lowpage = 0x30;

    if(w == IMode::R38) lowpage = 0x38;
    if(w == IMode::R8) lowpage = 0x08;
    if(w == IMode::R18) lowpage = 0x18;
    if(w == IMode::R28) lowpage = 0x28;

    SP -= 1;
    bus->Write(SP, PC >> 8);

    SP -= 1;
    bus->Write(SP, PC);

    PC = bus->Read(0x00) << 8 | bus->Read(lowpage);
    PC -= 1;

    return 0;
  }

  uint8_t CPU::RET(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    if(w == IMode::Z)
    {
      if(!Get_Bit_N(AF, Z_FLAG)) return 0;
    }
    
    if(w == IMode::NZ)
    {
      if(Get_Bit_N(AF, Z_FLAG)) return 0;
    }

    if(w == IMode::C)
    {
      if(!Get_Bit_N(AF, C_FLAG)) return 0;
    }
   
    if(w == IMode::NC)
    {
      if(Get_Bit_N(AF, C_FLAG)) return 0;
    }

    uint16_t addr = Combine(bus->Read(SP+1), bus->Read(SP));
    SP += 2;
    PC = addr - 1;
    
    return 12;
  }

  uint8_t CPU::RETI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    IME = true;

    uint16_t addr = Combine(bus->Read(SP+1), bus->Read(SP));
    SP += 2;
    PC = addr - 1;
     
    return 16;
  }

  uint8_t CPU::CPL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t A = AF >> 8;

    A = ~A;

    AF &= 0x00FF;
    AF |= A << 8;

    Set_Bit_N(&AF, N_FLAG, 1);
    Set_Bit_N(&AF, H_FLAG, 1);
    return 0;
  }

  uint8_t CPU::CCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);

    Set_Bit_N(&AF, C_FLAG, (bool)~Get_Bit_N(AF, C_FLAG));
    return 4;
  }

  uint8_t CPU::DAA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t flag_h = Get_Bit_N(AF, H_FLAG);
    uint8_t flag_c = Get_Bit_N(AF, C_FLAG);
    uint8_t flag_n = Get_Bit_N(AF, N_FLAG);

    uint8_t A = AF >> 8;

    uint8_t val = 0;
    uint8_t fc = 0;
    
    if(flag_h || (!flag_n && (A & 0xF) > 9))
    {
      val = 6;
    }

    if(flag_c || (!flag_n && A > 0x99))
    {
      val |= 0x60;
      fc = 1;
    }

    A += flag_n ? -val : val;

    AF &= 0x00FF;
    AF |= A << 8;
    
    Set_Bit_N(&AF, Z_FLAG, (bool)(A == 0));
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, fc);
    return 4;
  }

  uint8_t CPU::SCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 1);
    return 4;
  }

  uint8_t CPU::RRCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = AF >> 8;

    uint8_t bottom = Get_Bit_N(val, 0);

    val >>= 1;
    
    Set_Bit_N(&val, 7, bottom);
    
    AF &= 0x00FF;
    AF |= val << 8;

    Set_Bit_N(&AF, C_FLAG, bottom);
    
    Set_Bit_N(&AF, Z_FLAG, 0);
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);

    return 4;
  }

  uint8_t CPU::RRA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = AF >> 8;
    uint8_t bottom = Get_Bit_N(val, 0);
    uint8_t old_c = Get_Bit_N(AF, C_FLAG);
    
    val >>= 1;
    Set_Bit_N(&val, 7, (bool)old_c);
    
    AF &= 0x00FF;
    AF |= val << 8;
    
    Set_Bit_N(&AF, C_FLAG, bottom);

    Set_Bit_N(&AF, Z_FLAG, 0);
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 4;
  }
  
  uint8_t CPU::RLCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = AF >> 8;

    uint8_t top = Get_Bit_N(val, 7);

    val <<= 1;
    
    Set_Bit_N(&val, 0, top);
    
    AF &= 0x00FF;
    AF |= val << 8;

    Set_Bit_N(&AF, C_FLAG, top);
    
    Set_Bit_N(&AF, Z_FLAG, 0);
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);

    return 4;
  }

  uint8_t CPU::RLA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = AF >> 8;
    uint8_t top = Get_Bit_N(val, 7);
    uint8_t old_c = Get_Bit_N(AF, C_FLAG);
    
    val <<= 1;
    Set_Bit_N(&val, 0, (bool)old_c);
    
    AF &= 0x00FF;
    AF |= val << 8;
    
    Set_Bit_N(&AF, C_FLAG, top);

    Set_Bit_N(&AF, Z_FLAG, 0);
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 4;
  }
    
  uint8_t CPU::SBC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t cflag = Get_Bit_N(AF, C_FLAG);
    
    Set_Bit_N(&AF, N_FLAG, 1);

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    if(w == IMode::HIGH)
    {
      uint8_t A = *dest_register >> 8;

      Set_Half_Carry_MinusC(A, val);
      Set_Carry_MinusC(A, val);

      A -= val + cflag;

      *dest_register &= 0x00FF;
      *dest_register |= A << 8;

      Set_Bit_N(&AF, Z_FLAG, (bool)(A == 0));
    }
    
    return 0;
  }

  uint8_t CPU::ADC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t cflag = Get_Bit_N(AF, C_FLAG);
    
    Set_Bit_N(&AF, N_FLAG, 0);

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }

    uint8_t A = *dest_register >> 8;
    
    Set_Half_CarryC(A, val, true);
    Set_Carry_PlusC(A, val, true);

    A += (uint8_t)(val + cflag);

    *dest_register &= 0x00FF;
    *dest_register |= A << 8;

    Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)A == 0));
    return 0;
  }

  uint8_t CPU::CP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    Set_Bit_N(&AF, N_FLAG, 1);

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
    }

    if(r == IMode::N8)
    {
      PC += 1;
      val = bus->Read(PC);
    }
    
    uint8_t dest_val = *dest_register >> 8;
    
    uint16_t result = dest_val - val;

    Set_Bit_N(&AF, Z_FLAG, (bool)(result == 0));
    Set_Half_Carry_Minus(dest_val, val);
    Set_Carry_Minus(dest_val, val);

    return 0;
  }

  uint8_t CPU::RES(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;

    if(r == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *dest_register;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);

      Set_Bit_N(&val, (uint8_t)w, 0);

      bus->Write(*src_value, val);
      return 16;
    }

    Set_Bit_N(&val, (uint8_t)w, 0);

    
    if(r == IMode::HIGH)
    {
      *dest_register &= val << 8;
    }

    if(r == IMode::LOW)
    {
      *dest_register &= val;
    }
    
    return 8;
  }

  uint8_t CPU::SET(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;

    if(r == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(r == IMode::LOW)
    {
      val = *dest_register;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);

      Set_Bit_N(&val, (uint8_t)w, 0);

      bus->Write(*src_value, val);
      return 16;
    }

    Set_Bit_N(&val, (uint8_t)w, 1);

    
    if(r == IMode::HIGH)
    {
      *dest_register &= val << 8;
    }

    if(r == IMode::LOW)
    {
      *dest_register &= val;
    }
    
    return 8;
  }

  uint8_t CPU::IBIT(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 0;
    if(r == IMode::LOW)
    {
      val = *src_value;
    }

    if(r == IMode::HIGH)
    {
      val = *src_value >> 8;
    }

    if(r == IMode::MEM)
    {
      val = bus->Read(*src_value);
      n = 4;
    }

    uint8_t setv = Get_Bit_N(val, (uint8_t)w);
    
    Set_Bit_N(&AF, Z_FLAG, (bool)(setv == 0));
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 1);
    return 8 + n;
  }

  uint8_t CPU::SWAP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;
    
    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
      n = 2;
    }


    uint8_t low = val >> 4;
    low &= 0x0F;

    uint8_t high = val;
    high &= 0xF0;

    val &= 0x00;
    val = high | low;

    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= val << 8;
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
    }

    return 8 * n;
  }

  uint8_t CPU::SLA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t top = Get_Bit_N(val, 7);
    val <<= 1;
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, top);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }

  uint8_t CPU::SRA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t bottom = Get_Bit_N(val, 0);
    uint8_t top = Get_Bit_N(val, 7);
    val >>= 1;
    Set_Bit_N(&AF, 7, top);
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, bottom);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }

  uint8_t CPU::RR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t bottom = Get_Bit_N(val, 0);
    uint8_t old_c = Get_Bit_N(AF, C_FLAG);
    
    val >>= 1;
    Set_Bit_N(&val, 7, (bool)old_c);
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, bottom);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }

  uint8_t CPU::RL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t top = Get_Bit_N(val, 7);
    uint8_t old_c = Get_Bit_N(AF, C_FLAG);
    
    val <<= 1;
    Set_Bit_N(&val, 0, (bool)old_c);
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, top);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }

  uint8_t CPU::RRC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t bottom = Get_Bit_N(val, 0);
    val >>= 1;
    Set_Bit_N(&val, 7, (bool)bottom);
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, bottom);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }

  uint8_t CPU::SRL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t bottom = Get_Bit_N(val, 0);
    
    val >>= 1;
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, bottom);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }

  uint8_t CPU::RLC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint8_t val = 0;
    uint8_t n = 1;

    if(w == IMode::LOW)
    {
      val = *dest_register;
    }

    if(w == IMode::HIGH)
    {
      val = *dest_register >> 8;
    }

    if(w == IMode::MEM)
    {
      val = bus->Read(*dest_register);
    }

    uint8_t top = Get_Bit_N(val, 7);
    val <<= 1;
    Set_Bit_N(&val, 0, top);
    
    if(w == IMode::LOW)
    {
      *dest_register &= 0xFF00;
      *dest_register |= val;
    }

    if(w == IMode::HIGH)
    {
      *dest_register &= 0x00FF;
      *dest_register |= (val << 8);
    }

    if(w == IMode::MEM)
    {
      bus->Write(*dest_register, val);
      n = 2;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(val == 0));
    Set_Bit_N(&AF, C_FLAG, top);

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    return 8 * n;
  }
  
};
