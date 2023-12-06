#pragma once
#include "Sandbox.h"

#define GBC_LOG(x) std::cout << "[GameBoy] " << x << std::endl;

#define TFT_WIDTH 160
#define TFT_HEIGHT 144

namespace GBC
{
  template<typename T>
    class DIFO
  {
  public:
  
    DIFO()
    {

    }

    inline void pop()
    {
      if(data.size() != 0)
      {
	data.erase(data.begin());
      }
    }

    inline T front()
    {
      if(data.size() == 0)
      {
	std::cout << "What in the fucking hell are you doing" << std::endl;
	return T();
      }
      return data[0];
    }

    inline void push(T val)
    {
      data.push_back(val);
    }

    inline std::size_t size()
    {
      return data.size();
    }

  private:
    std::vector<T> data;
  };

  uint16_t Combine(uint8_t first, uint8_t second);
  uint16_t Get_Value_N(uint16_t opcode, uint8_t n);

  void Set_Bit_N(uint8_t *reg, uint8_t n, uint8_t val);
  void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
  uint16_t Get_Bit_N(uint16_t src, uint8_t n);

};
