#include "gbc/cpu.h"
#include "gbc/bitwise.h"

#include "Application.hpp"

#define Z_FLAG 7
#define N_FLAG 6
#define H_FLAG 5
#define C_FLAG 4

namespace GBC
{

  void CPU::Validate_Opcode()
  {
    // keep in mind: the GameBoy CPU (SM83) has Little-Endianness (reads multiple bytes backwards from ram)
    uint8_t opcode = bus->Read(PC);
    (this->*lookup[opcode].opfun)(lookup[opcode].dest, lookup[opcode].w, lookup[opcode].src, lookup[opcode].r);

    PC += 1;
  }

  // starts counting from LSB
  void CPU::Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val)
  {
    // todo: assert
    if(val > 1)
    {
      GBC_LOG("Cannot set bit. Val > 1");
      return;
    }
    if(n > 16)
    {
      GBC_LOG("Could not set bit. n > 16");
      return;
    }

    uint16_t n_val = *reg;
    n_val <<= 15 - n;
    n_val >>= 15;

    if(val == n_val)
    {
      return;
    }
    n_val = val;
    uint16_t s_reg = n_val;
    s_reg <<= n;
    s_reg |= *reg;

    *reg = s_reg;

    return;
  }

  void CPU::Set_Half_Carry(uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t erase_val = 0x0F;
    uint16_t check_val = 0x10;
    if(!bit8)
    {
      erase_val = 0x0F00;
      check_val = 0x1000;
    }

    src_register &= erase_val;
    val &= erase_val;
    
    src_register += val;
    if((src_register & check_val) == check_val)
    {
      Set_Bit_N(&AF, H_FLAG, 1);
    }      
    return;
  }

  void CPU::Set_Carry_Plus(uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t check_val = 0xFF;
    if(!bit8)
    {
      check_val = 0xFFFF;
    }

    if((int)(src_register + val) > check_val)
    {
      Set_Bit_N(&AF, C_FLAG, 1);
    }

    return;
  }

  void CPU::Set_Carry_Minus(uint16_t src_register, uint8_t val)
  {
    uint8_t A = src_register;
    
    if((int)(A - val) < 0)
    {
      Set_Bit_N(&AF, C_FLAG, 1);
    }
    
    return;
  }
  
  uint16_t CPU::Get_Bit_N(uint16_t src, uint8_t n)
  {
    if(n > 16)
    {
      GBC_LOG("Could not get bit. n > 16");
      return 0;
    }

    src <<= 15 - n;
    src >>= 15;

    return src;
  }

  uint8_t CPU::PREFIX(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    PC += 1;
  }

  uint8_t CPU::EI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::DI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

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
      val = (*src_value << 8) >> 8;
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
      val = *src_value + (int8_t)bus->Read(PC);
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
      bus->Write(val, *dest_register);
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
      
      bus->Write(addr, val);
      
    } 
    else // 0xF2
    {
      uint8_t val = bus->Read((uint8_t)(*src_value));
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
    
    if(w == IMode::HIGH)
    {
      Set_Half_Carry(*dest_register >> 8, 0x0001, true);
      *dest_register += 0x0100;
      Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));
      Set_Bit_N(&AF, N_FLAG, 0);
    }

    if(w == IMode::LOW)
    {
      Set_Half_Carry(*dest_register, 0x0001, true);
      *dest_register += 0x0001;
      Set_Bit_N(&AF, Z_FLAG, (bool)(((uint8_t)*dest_register) == 0));
      Set_Bit_N(&AF, N_FLAG, 0);
    }

    return 0;
  }
  
  uint8_t CPU::DEC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    if(w == IMode::ALL)
    {
      *dest_register -= 1;
    }
    
    if(w == IMode::HIGH)
    {
      Set_Half_Carry(*dest_register >> 8, -0x0100, true);
      *dest_register -= 0x0100;
      Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));
      Set_Bit_N(&AF, N_FLAG, 1);
    }

    if(w == IMode::LOW)
    {
      Set_Half_Carry(*dest_register, -0x0001, true);
      *dest_register -= 0x0001;
      Set_Bit_N(&AF, Z_FLAG, (bool)(((uint8_t)*dest_register) == 0));
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

    // 0xE8 special case
    if(r == IMode::E8)
    {
      PC += 1;
      val = bus->Read(PC);

      // set half carry
      {
	uint16_t erase_val = 0x0F00;
	uint16_t check_val = 0x1000;
	uint16_t src_register = *dest_register;
	int8_t modval = (int8_t)val;
	src_register &= erase_val;
	modval &= erase_val;
    
	src_register += modval;
	if((src_register & check_val) == check_val)
	{
	  Set_Bit_N(&AF, H_FLAG, 1);
	}
      }
      *dest_register += (int8_t)val;

      bool cflag = false;
      if(*dest_register < 0 || *dest_register > 0xFFFF)
      {
        cflag = true;
      }
      Set_Bit_N(&AF, C_FLAG, cflag);
      
      return 0;
    }

    if(w == IMode::HIGH)
    {
      Set_Half_Carry(*dest_register >> 8, val, true);
      Set_Carry_Plus(*dest_register >> 8, val, true);
      *dest_register += val;
      Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));
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
      Set_Half_Carry(*dest_register >> 8, -1 * val, false);
      Set_Carry_Minus(*dest_register >> 8, val);
      *dest_register -= val << 8;
      Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));
    }

    return 0;
  }

  uint8_t CPU::AND(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;

    if(r == IMode::HIGH)
    {
      val = (*src_value >> 8) << 8;
      val += 0x0F;
    }

    if(r == IMode::LOW)
    {
      val = *src_value << 8;
      val += 0x0F;
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
      *dest_register &= val;
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
      val = (*src_value >> 8) << 8;
    }

    if(r == IMode::LOW)
    {
      val = *src_value << 8;
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
      *dest_register |= val;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)((uint8_t)(*dest_register >> 8) == 0));

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 0;
  }

  // keep exception at 0xAF in mind
  uint8_t CPU::XOR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;

    if(r == IMode::HIGH)
    {
      val = (*src_value >> 8);
    }

    if(r == IMode::LOW)
    {
      val = (*src_value << 8) >> 8;
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
      uint8_t modify = ((*dest_register) >> 8) ^ val; 
      *dest_register &= 0x0F;
      *dest_register |= modify << 8;
    }

    if(w == IMode::LOW)
    {
      uint8_t modify = (uint8_t)*dest_register ^ val; 
      *dest_register &= 0xF0;
      *dest_register |= modify << 8;
    }

    if(w == IMode::ALL)
    {
      *dest_register ^= val;
    }

    Set_Bit_N(&AF, Z_FLAG, (bool)(*dest_register == 0));
    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 0;
  }

  uint8_t CPU::POP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    *dest_register &= bus->Read(SP);
    SP++;
    *dest_register &= (bus->Read(SP) << 8);
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

    if(r == IMode::E8)
    {
      PC += (int8_t)bus->Read(PC);
      PC -= 1;
    }

    return 4;
  }

  uint8_t CPU::CALL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    PC += 3;

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

  }

  uint8_t CPU::CPL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::CCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::DAA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::SCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::RRCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::RRA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::RLCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::RLA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
    
// keep exception at 0x9F in mind
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

    val -= cflag;
    
    if(w == IMode::HIGH)
    {
      Set_Half_Carry(*dest_register >> 8, -1 * val, true);
      Set_Carry_Minus(*dest_register >> 8, val);
      *dest_register -= val << 8;
    }
    
    Set_Bit_N(&AF, Z_FLAG, (bool)(*dest_register == 0));
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

    val += cflag;
    
    if(w == IMode::HIGH)
    {
      Set_Half_Carry(*dest_register >> 8, val, true);
      Set_Carry_Plus(*dest_register >> 8, val, true);
      *dest_register += val << 8;
    }
    
    Set_Bit_N(&AF, Z_FLAG, (bool)(*dest_register == 0));
    return 0;
  }

// keep exception at 0xBF in mind
  uint8_t CPU::CP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {
    uint16_t val = 0;
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

    uint16_t result = *dest_register - val;

    Set_Bit_N(&AF, Z_FLAG, (bool)(result == 0));
    Set_Half_Carry(*dest_register >> 8, -1 * val, true);
    Set_Carry_Minus(*dest_register >> 8, val);

    return 0;
  }
};
