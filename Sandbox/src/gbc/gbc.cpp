#include <cstring>
#include <sstream>
#include <cstdlib>
#include <time.h>

#include "gbc/gbc.h"
#include "gbc/bitwise.h"
#include "gbc/cpu.h"

namespace GBC 
{
  static int replace_first(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
    ) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return 0;
    s.replace(pos, toReplace.length(), replaceWith);
    return 1;
  }

  static std::string Hex_To_String(uint16_t val, const char* prefix)
  {
    std::string ret;
    std::stringstream hex;
    hex << std::hex << val;
    
    ret = std::string(prefix) + hex.str();

    return ret;
  }

  void Spec::add_address(int &address)
  {
    std::string mnemonic = cpu.lookup[bus.Read(address, false)].mnemonic;
    replace_first(mnemonic, "PREFIX", cpu.lookup_cb[bus.Read(address+1, false)].mnemonic);
    replace_first(mnemonic, "e8", std::string(Hex_To_String(bus.Read(address+1, false), "$(") + ")"));
    replace_first(mnemonic, "n8", std::string(Hex_To_String(bus.Read(address+1, false), "$(") + ")"));
    replace_first(mnemonic, "a8", std::string(Hex_To_String(bus.Read(address+1, false), "$(") + ")"));

    replace_first(mnemonic, "a16", std::string(Hex_To_String(bus.Read(address+2, false) << 8 | bus.Read(address+1, false), "$(") + ")"));

    replace_first(mnemonic, "n16", std::string(Hex_To_String(bus.Read(address+2, false) << 8 | bus.Read(address+1, false) , "$(") + ")"));

    instructions[address] = std::pair<uint16_t, std::string>(address, mnemonic);
    address++;
  }
  
  void Spec::Init(const char* rom_path)
  {
    rom.Init(rom_path);

    // imgui preparation
    int address = 0;
    while(address != 0x10000)
    {
      add_address(address);
    }
    cpu.state = State::RUN;
  }

  bool adt = false;
  bool post_bios = false;

  void Spec::Update()
  {
    if(cpu.PC == 0x0100 && !post_bios)
    {
      post_bios = true;
	
      rom.Post_Bios();
      int i = 0;
      while(i != 0x0100)
      {
	add_address(i);
      }
    }
    
    if(!adt)
    {
      adt = true;
    }

    if(cpu.state == State::RUN) //&& breakaddr != cpu.PC && dstate != Debug::STOP)
    {
      adt = false;
      cpu.Validate_Opcode();

      if(dstate == Debug::STEP)
      {
	dstate = Debug::STOP;
      }
    }

    if(cpu.state == State::HALT)
    {
      bus.Emulate_Cycle(4, true);
      if(bus.Read(A_IF, false) & bus.Read(A_IE, false))
      {
	cpu.state = State::RUN;
      }
    }

    if(cpu.IME)
    {
      interrupt.Handle(&cpu);
    }

    //io.Serial_Update();
  }
  
};
