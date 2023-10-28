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
    GBC_LOG("Loaded " + std::to_string(index - entry) + " bytes");

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
    uint8_t opcode = spec->ram[spec->PC];
    // splits up in multiple files
    // keep in mind: the GameBoy CPU (SM83) has Little-Endianness (reads multiple bytes backwards from ram)
    switch(opcode)
    {
    // NOP
    case 0x00:
    {
      // no operation
      break;
    }
    
    // LD BC, n16
    case 0x01:
    {
      spec->PC++;
      LD16(&spec->BC, Combine(spec->ram[spec->PC + 1], spec->ram[spec->PC]));
      break;
    }
    
    // LD [BC], A
    case 0x02:
    {
      uint8_t register_a = (spec->AF >> 4);
      
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
      INC8(&spec->AF, &spec->BC, true);
      break;
    }

    // DEC B
    case 0x05:
    {
      DEC8(&spec->AF, &spec->BC, true);
      break;
    }

    // LD B, n8
    case 0x06:
    {
      spec->PC++;
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
      spec->PC++;
      uint16_t address = Combine(spec->ram[spec->PC + 1], spec->ram[spec->PC]);
      spec->ram[address] = spec->SP;
      spec->ram[address+1] = (spec->SP >> 8);
      break;
    }

    // ADD HL, BC
    case 0x09:
    {
      ADD16(&spec->AF, &spec->HL, spec->BC);
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
      INC8(&spec->AF, &spec->BC, false);
      break;
    }

    // DEC C
    case 0x0D:
    {
      DEC8(&spec->AF, &spec->BC, false);
      break;
    }

    // LD C, n8
    case 0x0E:
    {
      spec->PC++;
      LD8(&spec->BC, spec->ram[spec->PC], false);
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
      spec->PC++;
      LD16(&spec->DE, Combine(spec->ram[spec->PC+1], spec->ram[spec->PC]));
      spec->PC++;
      break;
    }

    // LD [DE], A
    case 0x12:
    {
      spec->ram[spec->DE] = (spec->AF >> 4);
      break;
    }

    // INC DE
    case 0x13:
    {
      INC16(&spec->DE);
      break;
    }

    // INC D
    case 0x14:
    {
      INC8(&spec->AF, &spec->DE, true);
      break;
    }

    // DEC D
    case 0x15:
    {
      DEC8(&spec->AF, &spec->DE, true);
      break;
    }

    // LD D, n8
    case 0x16:
    {
      spec->PC++;
      LD8(&spec->DE, spec->ram[spec->PC], true);
      break;
    }

    // RLA
    case 0x17:
    {
      break;
    }

    // JR e8
    case 0x18:
    {
      spec->PC += spec->ram[spec->PC+1];
      spec->PC--;
      break;
    }

    // ADD HL, DE
    case 0x19:
    {
      ADD16(&spec->AF, &spec->HL, spec->DE);
      break;
    }

    // LD A, [DE]
    case 0x1A:
    {
      LD8(&spec->AF, spec->ram[spec->DE], true);
      break;
    }

    // DEC DE
    case 0x1B:
    {
      DEC16(&spec->DE);
      break;
    }

    // INC E
    case 0x1C:
    {
      INC8(&spec->AF, &spec->DE, false);
      break;
    }

    // DEC E
    case 0x1D:
    {
      DEC8(&spec->AF, &spec->DE, false);
      break;
    }

    // LD E, n8
    case 0x1E:
    {
      spec->PC++;
      LD8(&spec->DE, spec->ram[spec->PC], false);
      break;
    }

    // RRA
    case 0x1F:
    {
      break;
    }

    // JR NZ, e8
    case 0x20:
    {
      uint8_t z = spec->AF;
      z >>= 7;

      spec->PC++;

      if(!z)
      {
	spec->PC += spec->ram[spec->PC];
	spec->PC--;
      }
      break;
    }

    // LD HL, n16
    case 0x21:
    {
      spec->PC++;
      LD16(&spec->HL, Combine(spec->ram[spec->PC+1], spec->ram[spec->PC]));
      spec->PC++;
      break;
    }

    // LD [HL+], A
    case 0x22:
    {
      spec->ram[spec->HL] = (spec->AF >> 8);
      spec->HL += 1;
      break;
    }

    // INC HL
    case 0x23:
    {
      INC16(&spec->HL);
      break;
    }

    // INC H
    case 0x24:
    {
      INC8(&spec->AF, &spec->HL, true);
      break;
    }

    // DEC H
    case 0x25:
    {
      INC8(&spec->AF, &spec->HL, true);
      break;
    }

    // LD H, n8
    case 0x26:
    {
      spec->PC++;
      LD8(&spec->HL, spec->ram[spec->PC], true);
      break;
    }

    // DAA 
    case 0x27:
    {
      break;
    }

    // JR Z, e8
    case 0x28:
    {
      uint8_t z = spec->AF;
      z >>= 7;
      spec->PC++;

      if(z)
      {
	spec->PC += spec->ram[spec->PC];
	spec->PC--;
      }
      break;
    }

    // ADD HL, HL
    case 0x29:
    {
      spec->HL += spec->HL;
      break;
    }

    // LD A, [HL+]
    case 0x2A:
    {
      uint16_t AF = (spec->AF << 8);
      AF >>= 8;

      AF |= (spec->ram[spec->HL] << 8);
      
      spec->AF = AF;
      
      spec->HL += 1;
      
      break;
    }

    // DEC HL
    case 0x2B:
    {
      DEC16(&spec->HL);
      break;
    }

    // INC L
    case 0x2C:
    {
      INC8(&spec->HL, false);
      break;
    }

    // DEC L
    case 0x2D:
    {
      DEC8(&spec->HL, false);
      break;
    }

    // LD L, n8
    case 0x2E:
    {
      spec->PC++;
      LD8(&spec->HL, spec->ram[spec->PC], false);
      break;
    }

    // CPL
    case 0x2F:
    {
      break;
    }

    // JR NC, e8
    case 0x30:
    {
      uint8_t c = (spec->BC << 8) >> 8;

      spec->PC++;

      if(!c)
      {
	spec->PC += spec->ram[spec->PC];
	spec->PC--;
      }
      break;
    }

    // LD SP, n16
    case 0x31:
    {
      spec->PC++;
      LD16(&spec->SP, Combine(spec->ram[spec->PC+1], spec->ram[spec->PC]));
      spec->PC++;
      break;
    }

    // LD [HL-], A
    case 0x32:
    {
      spec->ram[spec->HL] = (spec->AF >> 8);
      spec->HL--;
      break;
    }

    // INC SP
    case 0x33:
    {
      INC16(&spec->SP);
      break;
    }

    // INC [HL]
    case 0x34:
    {
      spec->ram[spec->HL] += 1;
      break;
    }

    // DEC [HL]
    case 0x35:
    {
      spec->ram[spec->HL] -= 1;
      break;
    }

    // LD [HL], n8
    case 0x36:
    {
      spec-PC++;
      spec->ram[spec->HL] = spec->ram[spec->PC];
      break;
    }

    // SCF
    case 0x37:
    {
      break;
    }

    // JR C, e8
    case 0x38:
    {
      uint8_t c = (spec->BC << 8) >> 8;

      spec->PC++;

      if(c)
      {
	spec->PC += spec->ram[spec->PC];
	spec->PC--;
      }
      break;
    }

    // ADD HL, SP
    case 0x39:
    {
      spec->HL += spec->SP;
      break;
    }

    // LD A, [HL-]
    case 0x3A:
    {
      uint16_t AF = (spec->AF << 8);
      AF >>= 8;

      AF |= (spec->ram[spec->HL] << 8);
      
      spec->AF = AF;
      
      spec->HL -= 1;
      break;
    }

    // DEC SP
    case 0x3B:
    {
      DEC16(&spec->SP);
      break;
    }

    // INC A
    case 0x3C:
    {
      INC8(&spec->AF, true);
      break;
    }

    // DEC A
    case 0x3D:
    {
      DEC8(&spec->AF, true);
      break;
    }

    // LD A, n8
    case 0x3E:
    {
      spec->PC++;
      LD8(&spec->AF, spec->ram[spec->PC], true);
      break;
    }

    // CCF
    case 0x3F:
    {
      break;
    }
    
    default:
    {
      std::stringstream ss;
      ss << std::hex << opcode;
      GBC_LOG("Instruction " + ss.str() + " unknown");
      break;
    }
    }
    spec->PC++;
  }
  
  void Update(Spec *spec)
  {
    if(spec->state == State::RUN)
    {
      Validate_Opcode(spec);
    }
  }
  
};
