#include <cstring>
#include <sstream>
#include <cstdlib>

#include "gbc/gbc.h"
#include "gbc/opfunction.h"

namespace GBC 
{
  uint16_t Character_Address(uint8_t ch)
  {
    return ch * 5;
  }
  
  bool Load_Rom(Spec *spec, const char* rom_path)
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
      
    // put all 32 KiB into ram
    char byte = 0;
    uint16_t index = entry;
    while(file.get(byte))
    {
      spec->ram[index] = byte;
      index += 1;
    }
    GBC_LOG("Loaded " + std::to_string(length) + " bytes");

    file.close();
    return true;
  }

  uint16_t Combine(uint8_t first, uint8_t second)
  {
    uint16_t first_16 = (uint16_t) first; 
    uint16_t second_16 = (uint16_t) second;

    first_16 = first_16 << 8;

    uint16_t val = first_16 | second_16;

    return val;
  }

  uint16_t Get_Value_N(uint16_t opcode, uint8_t n)
  {
    if(n > 3)
    {
      GBC_LOG("Cannot get " + std::to_string(n) + ". Opcode is 16-bit only: 0-3 MAX");
      return 0;
    }
    opcode = opcode << (n * 4);
    opcode = opcode >> 12;

    return opcode;
  }
  
  void Init_Spec(Spec *spec, const char* rom_path)
  {
    // zero all arrays
    memset(spec->ram, 0, sizeof(spec->ram) / sizeof(spec->ram[0]));

    for(int y = 0; y < TFT_HEIGHT; y++)
    {
      for(int x = 0; x < TFT_WIDTH; x++)
      {
	spec->display[y][x] = 0;
      }
    }

    if(!Load_Rom(spec, rom_path))
    {
      GBC_LOG("Could not load ROM at: " + std::string(rom_path));
    }
    else
    {
      GBC_LOG("Loaded ROM '" + std::string(rom_path) + "'");
    }

    spec->state = State::RUN;
    spec->PC = entry; // set Program Counter to entry
    spec->rom = rom_path;
  }

  void Validate_Opcode(Spec *spec)
  {
    uint16_t opcode = Combine(spec->ram[spec->PC], spec->ram[spec->PC+1]);

    // splits up in multiple files
    switch(opcode)
    {
    case 0x00:
    {
      // no operation
      break;
    }
    
    // NOP
    case 0x01:
    {
      spec->PC += 2;
      LD16(&spec->BC, Combine(spec->ram[spec->PC], spec->ram[spec->PC + 1]));
      break;
    }
    
    // LD [BC], A
    case 0x02:
    {
      uint8_t register_a = (spec->AF >> 8);
      
      spec->PC += 2;
      spec->ram[spec->BC] = register_a;
      break;
    }

    // INC BC
    case 0x03:
    {
      INC16(&spec->BC);
      break;
    }

    // INC B
    case 0x04:
    {
      INC8(&spec->BC, true);
      break;
    }

    // DEC B
    case 0x05:
    {
      DEC8(&spec->BC, true);
      break;
    }

    // LD B, n8
    case 0x06:
    {
      spec->PC += 2;
      LD8(&spec->BC, spec->ram[spec->PC], true);
      break;
    }

    // 
    case 0x07:
    {
      //spec->PC += 2;
      break;
    }

    // LD [a16], SP
    case 0x08:
    {
      spec->PC += 2;
      uint16_t address = Combine(spec->ram[spec->PC + 1], spec->ram[spec->PC]);
      spec->ram[address] = spec->SP;
      break;
    }

    // ADD HL, BC
    case 0x09:
    {
      ADD16(&spec->HL, spec->BC);
      break;
    }

    // LD A, [BC]
    case 0x0A:
    {
      LD8(&spec->AF, spec->ram[spec->BC], true);
      break;
    }

    // DEC BC
    case 0x0B:
    {
      DEC16(&spec->BC);
      break;
    }

    // INC C
    case 0x0C:
    {
      INC8(&spec->BC, false);
      break;
    }

    // DEC C
    case 0x0D:
    {
      DEC8(&spec->BC, false);
      break;
    }

    // LD C, n8
    case 0x0E:
    {
      spec += 2;
      LD8(&spec->BC, spec->ram[spec->PC], false);
      spec--;
      break;
    }

    
    case 0x0F:
    {
      break;
    }

    // STOP n8
    case 0x10:
    {
      spec->PC += 2;
      spec->state = State::STOP;
      break;
    }

    // LD DE, n16
    case 0x11:
    {
      spec->PC += 2;
      LD16(&spec->DE, Combine(spec->ram[spec->PC+1], spec->ram[spec->PC]));
      break;
    }

    //
    
    default:
    {
      std::stringstream ss;
      ss << std::hex << opcode;
      GBC_LOG("Instruction " + ss.str() + " unknown");
      break;
    }
    }
    spec->PC += 2;
  }
  
  void Update(Spec *spec)
  {
    if(spec->state == State::RUN)
    {
      Validate_Opcode(spec);
    }
  }
  
};
