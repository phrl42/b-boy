#include "gbc/opfunction.h"
#include "gbc/bitwise.h"

#define Z_FLAG 7
#define N_FLAG 6
#define H_FLAG 5
#define C_FLAG 4

// IMPORTANT:

// OML I think I was drunk when I wrote this. I have to rewrite most of the opfunctions because they
// are written extremely complicated

namespace GBC
{
  // starts counting from LSB
  void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val)
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

  void Set_Half_Carry(uint16_t *flags_register, uint16_t src_register, uint16_t val, bool bit8)
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
      Set_Bit_N(flags_register, H_FLAG, 1);
    }      
    return;
  }

  void Set_Carry_Plus(uint16_t *flags_register, uint16_t src_register, uint16_t val, bool bit8)
  {
    uint16_t check_val = 0xFF;
    if(!bit8)
    {
      check_val = 0xFFFF;
    }

    if((int)(src_register + val) > check_val)
    {
      Set_Bit_N(flags_register, C_FLAG, 1);
    }

  }

  uint16_t Get_Bit_N(uint16_t src, uint8_t n)
  {
    if(n > 16)
    {
      GBC_LOG("Could not get bit. n > 16");
      return 0;
    }

    src <<= n - 15;
    src >>= 15;

    return src;
  }

  // Exceptions are in the switch statement. Such as: 0x08 (LD [a16], SP)
  // Relative Jumps are also there

  // LoaD 8-Bit value from RAM into Register
  void LD8(uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t l = reg;
      reg = src_value << 8 | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      reg = h << 8 | src_value;
    }
    *dest_register = reg;
    return;
  }

  // LoaD 16-Bit value from 
  void LD16(uint16_t *dest_register, uint16_t src_value)
  {
    *dest_register = src_value;
    return;
  }

  // Increment 8-Bit Register
  void INC8(uint16_t *flags_register, uint16_t *dest_register, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;

      Set_Half_Carry(flags_register, h, 1, true);
      h++;

      if(h == 0)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      uint8_t l = reg;
      reg = (h << 8) | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;
      l++;
      if(l == 0)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = (h << 8) | l;
    }

    Set_Bit_N(flags_register, N_FLAG, 0);
    *dest_register = reg;
    return;
  }
  
  // Increment 16-Bit Register
  void INC16(uint16_t *dest_register)
  {
    *dest_register++;
    return;
  }

  
  // Decrement 8-Bit Register
  void DEC8(uint16_t *flags_register, uint16_t *dest_register, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;
      h--;
      if(!h)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      uint8_t l = reg;
      reg = (h << 8) | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      l--;
      if(!l)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = (h << 8) | l;
    }

    Set_Bit_N(flags_register, N_FLAG, 1);

    *dest_register = reg;
    return;
  }

  // Decrement 16-Bit Register
  void DEC16(uint16_t *dest_register)
  {
    *dest_register--;
    return;
  }

  // ADD 8-Bit value to Register
  void ADD8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      h += src_value;
      if(!h)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = h << 8 | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;
      
      l += src_value;

      if(!l)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(flags_register, N_FLAG, 0);
    return;
  }

  // ADD 16-Bit value to Register
  void ADD16(uint16_t *flags_register, uint16_t *dest_register, uint16_t src_value)
  {
    *dest_register += src_value;

    Set_Bit_N(flags_register, N_FLAG, 0);
    return;
  }

  // SUB 8-Bit value from Register
  void SUB8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      h -= src_value;
      reg = h << 8 | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      l -= src_value;
      reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(flags_register, N_FLAG, 1);
    return;
  }

  
  // These ones only modify Register 'A' so I could remove the general part
  // Bitwise AND Register with value
  void AND8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      h &= src_value;
      if(!h)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = h << 8 | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      l &= src_value;
      if(!l)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(flags_register, N_FLAG, 0);
    Set_Bit_N(flags_register, H_FLAG, 1);
    Set_Bit_N(flags_register, C_FLAG, 0);
    return;
  }

  // Bitwise OR on Register with value
  void OR8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      h |= src_value;
      if(!h)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = h << 8 | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      l |= src_value;
      if(!l)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(flags_register, N_FLAG, 0);
    Set_Bit_N(flags_register, H_FLAG, 0);
    Set_Bit_N(flags_register, C_FLAG, 0);
    return;
  }

  // Bitwise XOR on Register with value
  // keep exception at 0xAF in mind
  void XOR8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      h ^= src_value;
      if(!h)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = h << 8 | l;
    }
    else
    {
      uint8_t h = reg >> 8;
      uint8_t l = reg;

      l ^= src_value;
      if(!l)
      {
	Set_Bit_N(flags_register, Z_FLAG, 1);
      }
      reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(flags_register, N_FLAG, 0);
    Set_Bit_N(flags_register, H_FLAG, 0);
    Set_Bit_N(flags_register, C_FLAG, 0);
  }

  
  // Custom operation on Register with 8-Bit value
  // keep exception at 0x9F in mind
  void SBC8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    *dest_register -= src_value;
    *dest_register -= Get_Bit_N(*flags_register, 4);
    
    Set_Bit_N(flags_register, N_FLAG, 1);
  }

  // Custom operation on Register with 8-Bit value
  void ADC8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    *dest_register += src_value;
    *dest_register += Get_Bit_N(*flags_register, 4);
     
    Set_Bit_N(flags_register, N_FLAG, 0);
  }

  // Custom operation on Register with 8-Bit value
  // keep exception at 0xBF in mind
  
  void CP8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half)
  {
    uint16_t cc = (*flags_register >> 8) - *dest_register;

    if(!cc)
    {
      Set_Bit_N(flags_register, Z_FLAG, 1);
    }
    Set_Bit_N(flags_register, N_FLAG, 1);
  }

};
