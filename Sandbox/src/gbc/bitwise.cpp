#include "gbc/bitwise.h"

namespace GBC
{
  // starts counting from LSB
  void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val)
  {
    // todo: assert
    if(val > 1)
    {
      GBC_LOG("Cannot set bit. Val > 1");
      printf("%d\n", val);
      return;
    }
    if(n > 15)
    {
      GBC_LOG("Could not set bit. n > 15");
      return;
    }

    uint16_t vals = 1 << n;

    if(val)
    {
      *reg |= vals;
    }
    else
    {
      *reg &= ~(vals);
    }
    return;
  }

  uint16_t Get_Bit_N(uint16_t src, uint8_t n)
  {
    if(n > 15)
    {
      GBC_LOG("Could not get bit. n > 15");
      return 0;
    }

    src <<= 15 - n;
    src >>= 15;

    return src;
  }

  void Set_Bit_N(uint8_t *reg, uint8_t n, uint8_t val)
  {
    // todo: assert
    if(val > 1)
    {
      GBC_LOG("Cannot set bit. Val > 1");
      printf("%d\n", val);
      return;
    }
    if(n > 7)
    {
      GBC_LOG("Could not set bit. n > 7");
      return;
    }

    uint8_t vals = 1 << n;
    
    if(val)
    {
      *reg |= vals;
    }
    else
    {
      *reg &= ~(vals);
    }
    
    return;
  }
 
  uint16_t Combine(uint8_t first, uint8_t second)
  {
    uint16_t first_16 = (uint16_t) first; 
    uint16_t second_16 = (uint16_t) second;

    first_16 = first_16 << 8;

    uint16_t val = first_16 | second_16;

    return val;
  }

};  
