#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>

#include "gbc/gbc.h"
#include "gbc/bitwise.h"
#include "gbc/cpu.h"

namespace GBC 
{
  static void replace_first(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
    ) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return;
    s.replace(pos, toReplace.length(), replaceWith);
  }

  static std::string Hex_To_String(uint16_t val, const char* prefix)
  {
    std::string ret;
    std::stringstream hex;
    hex << std::hex << val;
    
    ret = std::string(prefix) + hex.str();

    return ret;
  }

  void Spec::add_address(uint16_t address, std::string mnemonic)
  {
    for(auto pair : instructions)
    {
      if(address == pair.first)
      {
	return;
      }
    }

    replace_first(mnemonic, "PREFIX", cpu.lookup_cb[bus.Read(cpu.PC+1)].mnemonic);
    replace_first(mnemonic, "e8", std::string(Hex_To_String(bus.Read(address+1), "$(") + ")"));
    replace_first(mnemonic, "n8", std::string(Hex_To_String(bus.Read(address+1), "$(") + ")"));
    replace_first(mnemonic, "a8", std::string(Hex_To_String(bus.Read(address+1), "$(") + ")"));

    replace_first(mnemonic, "a16", std::string(Hex_To_String(bus.Read(address+2) << 8 | bus.Read(address+1), "$(") + ")"));

    replace_first(mnemonic, "n16", std::string(Hex_To_String(bus.Read(address+2) << 8 | bus.Read(address+1) , "$(") + ")"));
    instructions.push_back(std::pair<uint16_t, std::string>(address, mnemonic));
  }
  
  void Spec::Init(const char* rom_path)
  {
    rom.Init(rom_path);
    
    cpu.state = State::RUN;
  }

  void Spec::Update()
  {
    bool adt = false;
    while(!kill)
    {
      if(cpu.PC == 0x0100) rom.Post_Bios();
      
      if(breakop == bus.Read(cpu.PC))
      {
	dstate = Debug::STOP;
      }
      
      if(!adt)
      {
	adt = true;
	//add_address(cpu.PC, std::string(cpu.lookup[bus.Read(cpu.PC)].mnemonic));
      }

      if(cpu.state == State::RUN && breakaddr != cpu.PC && dstate != Debug::STOP)
      {
	adt = false;
	bus.PC = cpu.PC;
	
	cpu.Validate_Opcode();

	if(dstate == Debug::STEP)
	{
	  dstate = Debug::STOP;
	}
      }

      if(cpu.state == State::HALT)
      {
	bus.Emulate_Cycle(4, true);
	if(bus.Read(A_IF) & bus.Read(A_IE))
	{
	  cpu.state = State::RUN;
	}
      }
    
      if(cpu.IME)
      {
	interrupt.Handle(&cpu);
      }

      io.Serial_Update();
    }
  }
  
};
