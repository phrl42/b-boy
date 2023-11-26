#include <cstring>
#include <sstream>
#include <cstdlib>

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
  
  bool Spec::Load_Rom(const char* rom_path)
  {
    std::ifstream file(rom_path);
    
    if(!file)
    {
      return false;
    }

    // get file length
    int length = 0;
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
      
    char byte = 0;
    uint16_t index = 0;

    while(file.get(byte))
    {
      bus.Write(index, byte);
      index += 1;
    }

    GBC_LOG("Loaded " + std::to_string(index) + " bytes");

    index = 0;
    std::ifstream bootrom("assets/roms/bootrom.gb");
    while(bootrom.get(byte))
    {
      bus.Write(index, byte);
      index += 1;
    }

    if(index != entry) GBC_LOG("Loading BootRom failed.");

    file.close();
    return true;
  }

  
  void Spec::Init(const char* rom_path)
  {
    // zero all arrays
    for(int y = 0; y < TFT_HEIGHT; y++)
    {
      for(int x = 0; x < TFT_WIDTH; x++)
      {
	//ppu.display[y][x] = 0;
      }
    }

    if(!Load_Rom(rom_path))
    {
      GBC_LOG("Could not load ROM at: " + std::string(rom_path));
    }
    else
    {
      GBC_LOG("Loaded ROM '" + std::string(rom_path) + "'");
    }

    cpu.state = State::RUN;
    rom = rom_path;
  }

  void Spec::Update()
  {
    while(!kill)
    {
      uint8_t A = cpu.AF >> 8;
      uint8_t F = cpu.AF;

      uint8_t B = cpu.BC >> 8;
      uint8_t C = cpu.BC;

      uint8_t D = cpu.DE >> 8;
      uint8_t E = cpu.DE;

      uint8_t H = cpu.HL>> 8;
      uint8_t L = cpu.HL;

      printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X\n", A, F, B, C, D, E, H, L, cpu.SP, cpu.PC);

      if(cpu.state == State::RUN)
      {
	add_address(cpu.PC, std::string(cpu.lookup[bus.Read(cpu.PC)].mnemonic));
	cpu.Validate_Opcode();
	ppu.Render();
      }

      if(cpu.state == State::HALT)
      {
	if(bus.Read(A_IF))
	{
	  cpu.state = State::RUN;
	}
      }
    
      if(cpu.IME)
      {
	interrupt.Handle(&cpu);
      }

      io.Serial_Update();
      io.Serial_Print();
    }
  }
  
};
