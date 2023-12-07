#pragma once
#include "Sandbox.h"

#define GBC_LOG(x) std::cout << "[GameBoy] " << x << std::endl;

#define TFT_WIDTH 160
#define TFT_HEIGHT 144

namespace GBC
{

  struct FIFO
  {
    uint8_t bpp = 0;
    uint8_t obp = 0;

    // 7th bit of attribute flag
    uint8_t priority = 0;
  };
  
  class DIFO
  {
  public:
  
    DIFO()
      {

      }

    inline void pop()
      {
	if(data.size() == 0)
	{
	  std::cout << "[DIFO][pop] size == 0" << std::endl;
	  return;
	}
	try
	{
	  data.erase(data.begin());
	}
	catch(...)
	{

	}      
      }

    inline FIFO front()
      {
	if(data.size() == 0)
	{
	  std::cout << "[DIFO][front] size == 0" << std::endl;
	  return FIFO();
	}
	return data[0];
      }

    inline void clear()
      {
	data.clear();
      }
    
    inline void push(FIFO val)
      {
	try
	{
	  data.push_back(val);
	}
	catch(...)
	{

	}
      }

    inline std::size_t size()
      {
	return data.size();
      }

  private:
    std::vector<FIFO> data;
  };

  uint16_t Combine(uint8_t first, uint8_t second);
  uint16_t Get_Value_N(uint16_t opcode, uint8_t n);

  void Set_Bit_N(uint8_t *reg, uint8_t n, uint8_t val);
  void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
  uint16_t Get_Bit_N(uint16_t src, uint8_t n);

};
