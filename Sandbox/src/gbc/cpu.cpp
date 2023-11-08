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
    uint8_t opcode = bus->Read(PC);
    // keep in mind: the GameBoy CPU (SM83) has Little-Endianness (reads multiple bytes backwards from ram)
    /*switch(opcode)
      
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
    LD16(&BC, Combine(bus->Read(PC + 1), bus->Read(PC)));
    break;
    }
    
    // LD [BC], A
    case 0x02:
    {
    uint8_t CPU::register_a = (AF >> 4);
      
    PC += 2;
    bus->Write(BC, register_a);
    break;
    }

    // INC BC
    case 0x03:
    {
    INC16(&BC);
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
    spec->PC += (int8_t)spec->ram[spec->PC+1];
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
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    spec->PC++;

    if(!z)
    {
    spec->PC += (int8_t)spec->ram[spec->PC];
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
    uint8_t CPU::z = spec->AF;
    z >>= 7;
    spec->PC++;

    if(z)
    {
    spec->PC += (int8_t)spec->ram[spec->PC];
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
    INC8(&spec->AF, &spec->HL, false);
    break;
    }

    // DEC L
    case 0x2D:
    {
    DEC8(&spec->AF, &spec->HL, false);
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
    uint8_t CPU::c = spec->AF;
    c <<= 3;
    c >>= 7;
      
    spec->PC++;

    if(!c)
    {
    spec->PC += (int8_t)spec->ram[spec->PC];
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
    spec->PC++;
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
    uint8_t CPU::c = spec->AF;
    c <<= 3;
    c >>= 7;

    spec->PC++;

    if(c)
    {
    spec->PC += (int8_t)spec->ram[spec->PC];
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
    INC8(&spec->AF, &spec->AF, true);
    break;
    }

    // DEC A
    case 0x3D:
    {
    DEC8(&spec->AF, &spec->AF, true);
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

    // LD B, B
    case 0x40:
    {
    LD8(&spec->BC, spec->BC >> 8, true);
    break;
    }

    // LD B, C
    case 0x41:
    {
    LD8(&spec->BC, spec->BC, true);
    break;
    }

    // LD B, D
    case 0x42:
    {
    LD8(&spec->BC, spec->DE >> 8, true);
    break;
    }

    // LD B, E
    case 0x43:
    {
    LD8(&spec->BC, spec->DE, true);
    break;
    }

    // LD B, H
    case 0x44:
    {
    LD8(&spec->BC, spec->HL >> 8, true);
    break;
    }

    // LD B, L
    case 0x45:
    {
    LD8(&spec->BC, spec->HL, true);
    break;
    }

    // LD B, [HL]
    case 0x46:
    {
    LD8(&spec->BC, spec->ram[spec->HL], true);
    break;
    }

    // LD B, A
    case 0x47:
    {
    LD8(&spec->BC, spec->AF >> 8, true);
    break;
    }

    // LD C, B
    case 0x48:
    {
    LD8(&spec->BC, spec->BC >> 8, false);
    break;
    }

    // LD C, C
    case 0x49:
    {
    LD8(&spec->BC, spec->BC, false);
    break;
    }

    // LD C, D
    case 0x4a:
    {
    LD8(&spec->BC, spec->DE >> 8, false);
    break;
    }

    // LD C, E
    case 0x4b:
    {
    LD8(&spec->BC, spec->DE, false);
    break;
    }

    // LD C, H
    case 0x4c:
    {
    LD8(&spec->BC, spec->HL >> 8, false);
    break;
    }

    // LD C, L
    case 0x4d:
    {
    LD8(&spec->BC, spec->HL, false);
    break;
    }

    // LD C, [HL]
    case 0x4e:
    {
    LD8(&spec->BC, spec->ram[spec->HL], false);
    break;
    }

    // LD C, A
    case 0x4f:
    {
    LD8(&spec->BC, spec->AF >> 8, false);
    break;
    }

    // LD D, B
    case 0x50:
    {
    LD8(&spec->DE, spec->BC >> 8, true);
    break;
    }

    // LD D, C
    case 0x51:
    {
    LD8(&spec->DE, spec->BC, true);
    break;
    }

    // LD D, D
    case 0x52:
    {
    LD8(&spec->DE, spec->DE >> 8, true);
    break;
    }

    // LD D, E
    case 0x53:
    {
    LD8(&spec->DE, spec->DE, true);
    break;
    }

    // LD D, H
    case 0x54:
    {
    LD8(&spec->DE, spec->HL >> 8, true);
    break;
    }

    // LD D, L
    case 0x55:
    {
    LD8(&spec->DE, spec->HL, true);
    break;
    }

    // LD D, [HL]
    case 0x56:
    {
    LD8(&spec->DE, spec->ram[spec->HL], true);
    break;
    }

    // LD D, A
    case 0x57:
    {
    LD8(&spec->DE, spec->AF >> 8, true);
    break;
    }

    // LD E, B
    case 0x58:
    {
    LD8(&spec->DE, spec->BC >> 8, false);
    break;
    }

    // LD E, C
    case 0x59:
    {
    LD8(&spec->DE, spec->BC, false);
    break;
    }

    // LD E, D
    case 0x5a:
    {
    LD8(&spec->DE, spec->DE >> 8, false);
    break;
    }

    // LD E, E
    case 0x5b:
    {
    LD8(&spec->DE, spec->DE, false);
    break;
    }

    // LD E, H
    case 0x5c:
    {
    LD8(&spec->DE, spec->HL >> 8, false);
    break;
    }

    // LD E, L
    case 0x5d:
    {
    LD8(&spec->DE, spec->HL, false);
    break;
    }

    // LD E, [HL]
    case 0x5e:
    {
    LD8(&spec->DE, spec->ram[spec->HL], false);
    break;
    }

    // LD E, A
    case 0x5f:
    {
    LD8(&spec->DE, spec->AF, false);
    break;
    }

    // LD H, B
    case 0x60:
    {
    LD8(&spec->HL, spec->BC >> 8, true);
    break;
    }

    // LD H, C 
    case 0x61:
    {
    LD8(&spec->HL, spec->BC, true);
    break;
    }

    // LD H, D
    case 0x62:
    {
    LD8(&spec->HL, spec->DE >> 8, true);
    break;
    }

    // LD H, E
    case 0x63:
    {
    LD8(&spec->HL, spec->DE, true);
    break;
    }

    // LD H, H
    case 0x64:
    {
    LD8(&spec->HL, spec->HL >> 8, true);
    break;
    }

    // LD H, L
    case 0x65:
    {
    LD8(&spec->HL, spec->HL, true);
    break;
    }

    // LD H, [HL]
    case 0x66:
    {
    LD8(&spec->HL, spec->ram[spec->HL], true);
    break;
    }

    // LD H, A
    case 0x67:
    {
    LD8(&spec->HL, spec->AF >> 8, true);
    break;
    }

    // LD L, B
    case 0x68:
    {
    LD8(&spec->HL, spec->BC >> 8, false);
    break;
    }

    // LD L, C
    case 0x69:
    {
    LD8(&spec->HL, spec->BC, false);
    break;
    }

    // LD L, D
    case 0x6a:
    {
    LD8(&spec->HL, spec->DE >> 8, false);
    break;
    }

    // LD L, E
    case 0x6b:
    {
    LD8(&spec->HL, spec->DE, false);
    break;
    }

    // LD L, H
    case 0x6c:
    {
    LD8(&spec->HL, spec->HL >> 8, false);
    break;
    }

    // LD L, L
    case 0x6d:
    {
    LD8(&spec->HL, spec->HL, false);
    break;
    }

    // LD L, [HL]
    case 0x6e:
    {
    LD8(&spec->HL, spec->ram[spec->HL], false);
    break;
    }

    // LD L, A
    case 0x6f:
    {
    LD8(&spec->HL, spec->AF >> 8, false);
    break;
    }

    // LD [HL], B
    case 0x70:
    {
    spec->ram[spec->HL] = spec->BC >> 8;
    break;
    }

    // LD [HL], C
    case 0x71:
    {
    spec->ram[spec->HL] = spec->BC;
    break;
    }

    // LD [HL], D
    case 0x72:
    {
    spec->ram[spec->HL] = spec->DE >> 8;
    break;
    }

    // LD [HL], E
    case 0x73:
    {
    spec->ram[spec->HL] = spec->DE;
    break;
    }

    // LD [HL], H
    case 0x74:
    {
    spec->ram[spec->HL] = spec->HL >> 8;
    break;
    }

    // LD [HL], L
    case 0x75:
    {
    spec->ram[spec->HL] = spec->HL;
    break;
    }

    // HALT
    case 0x76:
    {
    spec->state = State::HALT;
    break;
    }
    
    // LD [HL], A
    case 0x77:
    {
    spec->ram[spec->HL] = spec->AF >> 8;
    break;
    }

    // LD A, B
    case 0x78:
    {
    LD8(&spec->AF, spec->BC >> 8, true);
    break;
    }

    // LD A, C
    case 0x79:
    {
    LD8(&spec->AF, spec->BC, true);
    break;
    }

    // LD A, D
    case 0x7a:
    {
    LD8(&spec->AF, spec->DE >> 8, true);
    break;
    }

    // LD A, E
    case 0x7b:
    {
    LD8(&spec->AF, spec->DE, true);
    break;
    }
    
    // LD A, H
    case 0x7c:
    {
    LD8(&spec->AF, spec->HL >> 8, true);
    break;
    }

    // LD A, L
    case 0x7d:
    {
    LD8(&spec->AF, spec->HL, true);
    break;
    }

    // LD A, [HL]
    case 0x7e:
    {
    LD8(&spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // LD A, A
    case 0x7f:
    {
    LD8(&spec->AF, spec->AF >> 8, true);
    break;
    }

    // ADD A, B
    case 0x80:
    {
    ADD8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // ADD A, C
    case 0x81:
    {
    ADD8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // ADD A, D
    case 0x82:
    {
    ADD8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }
    
    // ADD A, E
    case 0x83:
    {
    ADD8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // ADD A, H
    // sus
    case 0x84:
    {
    ADD8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // ADD A, L
    case 0x85:
    {
    ADD8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // ADD A, [HL]
    case 0x86:
    {
    ADD8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }
    
    // ADD A, A
    case 0x87:
    {
    ADD8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // ADC A, B
    case 0x88:
    {
    ADD8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // ADC A, C
    case 0x89:
    {
    ADD8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // ADC A, D
    case 0x8a:
    {
    ADD8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // ADC A, E
    case 0x8b:
    {
    ADD8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // ADC A, H
    case 0x8c:
    {
    ADD8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // ADC A, L
    case 0x8d:
    {
    ADD8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // ADC A, [HL]
    case 0x8e:
    {
    ADD8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // ADC A, A
    case 0x8f:
    {
    ADD8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // SUB A, B
    case 0x90:
    {
    SUB8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // SUB A, C
    case 0x91:
    {
    SUB8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // SUB A, D
    case 0x92:
    {
    SUB8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // SUB A, E
    case 0x93:
    {
    SUB8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // SUB A, H
    case 0x94:
    {
    SUB8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // SUB A, L
    case 0x95:
    {
    SUB8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // SUB A, [HL]
    case 0x96:
    {
    SUB8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // SUB A, A
    case 0x97:
    {
    SUB8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // SBC A, B
    case 0x98:
    {
    SBC8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // SBC A, C
    case 0x99:
    {
    SBC8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // SBC A, D
    case 0x9a:
    {
    SBC8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // SBC A, E
    case 0x9b:
    {
    SBC8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // SBC A, H
    case 0x9c:
    {
    SBC8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // SBC A, L
    case 0x9d:
    {
    SBC8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // SBC A, [HL]
    case 0x9e:
    {
    SBC8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // SBC A, A
    case 0x9f:
    {
    SBC8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // AND A, B
    case 0xa0:
    {
    AND8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // AND A, C
    case 0xa1:
    {
    AND8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // AND A, D
    case 0xa2:
    {
    AND8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // AND A, E
    case 0xa3:
    {
    AND8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // AND A, H
    case 0xa4:
    {
    AND8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // AND A, L
    case 0xa5:
    {
    AND8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // AND A, [HL]
    case 0xa6:
    {
    AND8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // AND A, A
    case 0xa7:
    {
    AND8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // XOR A, B
    case 0xa8:
    {
    XOR8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // XOR A, C
    case 0xa9:
    {
    XOR8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // XOR A, D
    case 0xaa:
    {
    XOR8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // XOR A, E
    case 0xab:
    {
    XOR8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // XOR A, H
    case 0xac:
    {
    XOR8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // XOR A, L
    case 0xad:
    {
    XOR8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // XOR A, [HL]
    case 0xae:
    {
    XOR8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // XOR A, A
    case 0xaf:
    {
    XOR8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // OR A, B
    case 0xb0:
    {
    OR8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // OR A, C
    case 0xb1:
    {
    OR8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // OR A, D
    case 0xb2:
    {
    OR8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // OR A, E
    case 0xb3:
    {
    OR8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // OR A, H
    case 0xb4:
    {
    OR8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // OR A, L
    case 0xb5:
    {
    OR8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // OR A, [HL]
    case 0xb6:
    {
    OR8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // OR A, A
    case 0xb7:
    {
    OR8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // CP A, B
    case 0xb8:
    {
    CP8(&spec->AF, &spec->AF, spec->BC >> 8, true);
    break;
    }

    // CP A, C
    case 0xb9:
    {
    CP8(&spec->AF, &spec->AF, spec->BC, true);
    break;
    }

    // CP A, D
    case 0xba:
    {
    CP8(&spec->AF, &spec->AF, spec->DE >> 8, true);
    break;
    }

    // CP A, E
    case 0xbb:
    {
    CP8(&spec->AF, &spec->AF, spec->DE, true);
    break;
    }

    // CP A, H
    case 0xbc:
    {
    CP8(&spec->AF, &spec->AF, spec->HL >> 8, true);
    break;
    }

    // CP A, L
    case 0xbd:
    {
    CP8(&spec->AF, &spec->AF, spec->HL, true);
    break;
    }

    // CP A, [HL]
    case 0xbe:
    {
    CP8(&spec->AF, &spec->AF, spec->ram[spec->HL], true);
    break;
    }

    // CP A, A
    case 0xbf:
    {
    CP8(&spec->AF, &spec->AF, spec->AF >> 8, true);
    break;
    }

    // RET NZ
    case 0xc0:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    if(!z)
    {
    uint8_t CPU::c = spec->ram[spec->SP];
    spec->SP++;
    uint8_t CPU::p = spec->ram[spec->SP];
    spec->SP++;

    spec->PC = (p << 8) | c;
    spec->PC--;
    }
    break;
    }

    // POP BC
    case 0xc1:
    {
    LD8(&spec->BC, spec->ram[spec->SP], false);
    spec->SP++;
    LD8(&spec->BC, spec->ram[spec->SP], true);
    spec->SP++;
    break;
    }

    // JP NZ, a16
    case 0xc2:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    if(!z)
    {
    uint16_t address = 0;
    spec->PC++;
    address = (spec->ram[spec->PC+1] << 8) | spec->ram[spec->PC];

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // JP a16
    case 0xc3:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    uint16_t address = 0;
    spec->PC++;
    address = (spec->ram[spec->PC+1] << 8) | spec->ram[spec->PC];

    spec->PC = address;
    spec->PC--;
    break;
    }

    // CALL NZ, a16
    case 0xc4:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    if(!z)
    {
    spec->PC++;
    uint16_t address = (spec->ram[spec->PC+1] << 8) || spec->ram[spec->PC];
    spec->ram[spec->SP+1] = spec->PC+2;
    spec->ram[spec->SP+2] = spec->PC+2 >> 8;

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // PUSH BC
    case 0xc5:
    {
    spec->ram[spec->SP-1] = spec->BC >> 8;
    spec->ram[spec->SP-2] = spec->BC;
    spec->SP -= 2;
    break;
    }

    // ADD A, n8
    case 0xc6:
    {
    spec->PC++;
    ADD8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $00
    case 0xc7:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0] << 8) | (spec->ram[0]);
    spec->PC--;
    break;
    }

    // RET Z
    case 0xc8:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    if(z)
    {
    uint8_t CPU::c = spec->ram[spec->SP];
    spec->SP++;
    uint8_t CPU::p = spec->ram[spec->SP];
    spec->SP++;

    spec->PC = (p << 8) | c;
    spec->PC--;
    }
    break;
    }

    // RET
    case 0xc9:
    {
    uint8_t CPU::c = spec->ram[spec->SP];
    spec->SP++;
    uint8_t CPU::p = spec->ram[spec->SP];
    spec->SP++;

    spec->PC = (p << 8) | c;
    spec->PC--;
    break;
    }

    // JP Z, a16
    case 0xca:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    if(z)
    {
    uint16_t address = 0;
    spec->PC++;
    address = (spec->ram[spec->PC+1] << 8) | spec->ram[spec->PC];

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // pain incoming
    case 0xcb:
    {
    break;
    }

    // CALL Z, a16
    case 0xcc:
    {
    uint8_t CPU::z = spec->AF;
    z >>= 7;

    if(z)
    {
    spec->PC++;
    uint16_t address = (spec->ram[spec->PC+1] << 8) || spec->ram[spec->PC];
    spec->ram[spec->SP+1] = spec->PC+2;
    spec->ram[spec->SP+2] = spec->PC+2 >> 8;

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // CALL a16
    case 0xcd:
    {
    spec->PC++;
    uint16_t address = (spec->ram[spec->PC+1] << 8) || spec->ram[spec->PC];
    spec->ram[spec->SP+1] = spec->PC+2;
    spec->ram[spec->SP+2] = spec->PC+2 >> 8;

    spec->PC = address;
    spec->PC--;
    break;
    }

    // ADC A, n8
    case 0xce:
    {
    spec->PC++;
    ADC8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $08
    case 0xcf:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0x00] << 8) | (spec->ram[0x08]);
    spec->PC--;
    break;
    }

    // RET NC
    case 0xd0:
    {
    uint8_t CPU::rc = spec->AF;
    rc <<= 3;
    rc >>= 7;
      
    if(!rc)
    {
    uint8_t CPU::c = spec->ram[spec->SP];
    spec->SP++;
    uint8_t CPU::p = spec->ram[spec->SP];
    spec->SP++;

    spec->PC = (p << 8) | c;
    spec->PC--;
    }
    break;
    }

    // POP DE
    case 0xd1:
    {
    LD8(&spec->DE, spec->ram[spec->SP], false);
    spec->SP++;
    LD8(&spec->DE, spec->ram[spec->SP], true);
    spec->SP++;
    break;
    }

    // JP NC, a16
    case 0xd2:
    {
    uint8_t CPU::c = spec->AF;
    c <<= 3;
    c >>= 7;

    if(!c)
    {
    uint16_t address = 0;
    spec->PC++;
    address = (spec->ram[spec->PC+1] << 8) | spec->ram[spec->PC];

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // -
    case 0xd3:
    {
    break;
    }

    // CALL NC, a16
    case 0xd4:
    {
    uint8_t CPU::c = spec->AF;
    c <<= 3;
    c >>= 7;
      
    if(!c)
    {
    spec->PC++;
    uint16_t address = (spec->ram[spec->PC+1] << 8) || spec->ram[spec->PC];
    spec->ram[spec->SP+1] = spec->PC+2;
    spec->ram[spec->SP+2] = spec->PC+2 >> 8;

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // PUSH DE
    case 0xd5:
    {
    spec->ram[spec->SP-1] = spec->DE >> 8;
    spec->ram[spec->SP-2] = spec->DE;
    spec->SP -= 2;
    break;
    }

    // SUB A, n8
    case 0xd6:
    {
    spec->PC++;
    SUB8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $10
    case 0xd7:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0x00] << 8) | (spec->ram[0x10]);
    spec->PC--;
    break;
    }

    // RET C
    case 0xd8:
    {
    uint8_t CPU::rc = spec->AF;
    rc <<= 3;
    rc >>= 7;

    if(rc)
    {
    uint8_t CPU::c = spec->ram[spec->SP];
    spec->SP++;
    uint8_t CPU::p = spec->ram[spec->SP];
    spec->SP++;

    spec->PC = (p << 8) | c;
    spec->PC--;
    }
    break;
    }

    // RETI
    case 0xd9:
    {
      
    break;
    }

    // JP C, a16
    case 0xda:
    {
    uint8_t CPU::c = spec->AF;
    c <<= 3;
    c >>= 7;

    if(c)
    {
    uint16_t address = 0;
    spec->PC++;
    address = (spec->ram[spec->PC+1] << 8) | spec->ram[spec->PC];

    spec->PC = address;
    spec->PC--;
    }
    break;
    }
    
    // -
    case 0xdb:
    {
    break;
    }

    // CALL C, a16
    case 0xdc:
    {
    uint8_t CPU::c = spec->AF;
    c <<= 3;
    c >>= 7;
      
    if(c)
    {
    spec->PC++;
    uint16_t address = (spec->ram[spec->PC+1] << 8) || spec->ram[spec->PC];
    spec->ram[spec->SP+1] = spec->PC+2;
    spec->ram[spec->SP+2] = spec->PC+2 >> 8;

    spec->PC = address;
    spec->PC--;
    }
    break;
    }

    // -
    case 0xdd:
    {
    break;
    }

    // SBC A, n8
    case 0xde:
    {
    spec->PC++;
    SBC8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $18
    case 0xdf:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0x00] << 8) | (spec->ram[0x18]);
    spec->PC--;
    break;
    }

    // LDH [a8], A
    case 0xe0:
    {
    spec->PC++;
    uint8_t CPU::a8 = spec->ram[spec->PC];

    spec->ram[0xFF00 + a8] = spec->AF >> 8;
    break;
    }

    // POP HL
    case 0xe1:
    {
    LD8(&spec->HL, spec->ram[spec->SP], false);
    spec->SP++;
    LD8(&spec->HL, spec->ram[spec->SP], true);
    spec->SP++;
    break;
    }

    // LD [C], A
    case 0xe2:
    {
    spec->ram[spec->BC] = spec->AF >> 8;
    break;
    }

    // -
    case 0xe3:
    {
    break;
    }

    // -
    case 0xe4:
    {
    break;
    }

    // PUSH HL
    case 0xe5:
    {
    spec->ram[spec->SP-1] = spec->HL >> 8;
    spec->ram[spec->SP-2] = spec->HL;
    spec->SP -= 2; 
    break;
    }

    // AND A, n8
    case 0xe6:
    {
    spec->PC++;
    AND8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $20
    case 0xe7:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0x00] << 8) | (spec->ram[0x20]);
    spec->PC--;
    break;
    }

    // ADD SP, e8
    // flags tbd 
    case 0xe8:
    {
    spec->PC++;
    ADD16(&spec->AF, &spec->SP, (int8_t)spec->ram[spec->PC]);
    break;
    }

    // JP HL
    case 0xe9:
    {
    spec->PC = spec->HL - 1;
    break;
    }

    // LD [a16], A
    case 0xea:
    {
    spec->PC++;
    uint16_t address = (spec->ram[spec->PC+1] << 8) |spec->ram[spec->PC];
    spec->ram[address] = spec->AF >> 8;
    spec->PC++;
    break;
    }

    // -
    case 0xeb:
    {
    break;
    }

    // -
    case 0xec:
    {
    break;
    }

    // -
    case 0xed:
    {
    break;
    }

    // SBC A, n8
    case 0xee:
    {
    spec->PC++;
    SBC8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $28
    case 0xef:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0x00] << 8) | (spec->ram[0x28]);
    spec->PC--;
    break;
    }

    // LDH A, [a8]
    case 0xf0:
    {
    spec->PC++;
    uint8_t CPU::F = spec->AF;
    uint8_t CPU::A = spec->ram[spec->PC] + 0xFF00;
    spec->AF = A << 8 | F;
    break;
    }

    // POP AF
    case 0xf1:
    {
    LD8(&spec->AF, spec->ram[spec->SP], false);
    spec->SP++;
    LD8(&spec->AF, spec->ram[spec->SP], true);
    spec->SP++;
    break;
    }

    // LD A, [C]
    case 0xf2:
    {
    LD8(&spec->AF, spec->ram[spec->BC], true);
    break;
    }

    // DI
    case 0xf3:
    {
    break;
    }

    // -
    case 0xf4:
    {
    break;
    }

    // PUSH AF
    case 0xf5:
    {
    spec->ram[spec->SP-1] = spec->AF >> 8;
    spec->ram[spec->SP-2] = spec->AF;
    spec->SP -= 2;
    break;
    }

    // OR A, n8
    case 0xf6:
    {
    spec->PC++;
    OR8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $30
    case 0xf7:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0x00] << 8) | (spec->ram[0x30]);
    spec->PC--;
    break;
    }

    // LD HL, SP + e8
    case 0xf8:
    {
    spec->PC++;
    LD16(&spec->HL, spec->SP + (int8_t)spec->ram[spec->PC]);
    break;
    }

    // LD SP, HL
    case 0xf9:
    {
    LD16(&spec->SP, spec->HL);
    break;
    }

    // LD A, [a16]
    case 0xfa:
    {
    spec->PC++;
    uint16_t address = spec->ram[spec->PC+1] << 8 | spec->ram[spec->PC];
    LD8(&spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // EI
    case 0xfb:
    {
    break;
    }

    // -
    case 0xfc:
    {
    break;
    }

    // -
    case 0xfd:
    {
    break;
    }

    // CP A, n8
    case 0xfe:
    {
    spec->PC++;
    CP8(&spec->AF, &spec->AF, spec->ram[spec->PC], true);
    break;
    }

    // RST $38
    case 0xff:
    {
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC >> 8;
    spec->SP -= 1;
    spec->ram[spec->SP] = spec->PC;

    spec->PC = (spec->ram[0] << 8) | (spec->ram[0x38]);
    spec->PC--;
    break;
    }
    
    default:
    {
    printf("[GameBoy]: Instruction 0x%x unknown\n", opcode);
    break;
    }
    }
    spec->PC++;
    */
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

  }

  void CPU::Set_Carry_Minus(uint16_t src_register, uint16_t val)
  {
    if((int)(src_register - val) < 0)
    {
      Set_Bit_N(&AF, C_FLAG, 1);
    }
  }
  
  uint16_t CPU::Get_Bit_N(uint16_t src, uint8_t n)
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
  /*uint8_t CPU::CPU::LD8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::l = reg;
    reg = src_value << 8 | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    reg = h << 8 | src_value;
    }
    *dest_register = reg;
    return 1;
    }

    // LoaD 16-Bit value from 
    uint8_t CPU::CPU::LD16(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    *dest_register = src_value;
    return 1;
    }

    // Increment 8-Bit Register
    uint8_t CPU::CPU::INC8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;

    Set_Half_Carry(h, 1, true);
    h++;

    if(h == 0)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    uint8_t CPU::l = reg;
    reg = (h << 8) | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;
    l++;
    if(l == 0)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = (h << 8) | l;
    }

    Set_Bit_N(&AF, N_FLAG, 0);
    *dest_register = reg;
    return 1;
    }
  
    // Increment 16-Bit Register
    uint8_t CPU::CPU::INC16(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    *dest_register++;
    return 1;
    }

  
    // Decrement 8-Bit Register
    uint8_t CPU::CPU::DEC8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;

    Set_Half_Carry(h, -1, true);

    h--;
    if(!h)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    uint8_t CPU::l = reg;
    reg = (h << 8) | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    Set_Half_Carry(l, -1, true);

    l--;
    if(!l)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = (h << 8) | l;
    }

    Set_Bit_N(&AF, N_FLAG, 1);

    *dest_register = reg;
    return 1;
    }

    // Decrement 16-Bit Register
    uint8_t CPU::CPU::DEC16(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    *dest_register--;
    return 1;
    }

    // ADD 8-Bit value to Register
    uint8_t CPU::CPU::ADD8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    Set_Carry_Plus(h, src_value, true);
    Set_Half_Carry(h, src_value, true);
      
    h += src_value;
    if(!h)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = h << 8 | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    Set_Carry_Plus(l, src_value, true);
    Set_Half_Carry(l, src_value, true);
    l += src_value;

    if(!l)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(&AF, N_FLAG, 0);
    return 1;
    }

    // ADD 16-Bit value to Register
    uint8_t CPU::CPU::ADD16(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    Set_Half_Carry(*dest_register, src_value, false);
    Set_Carry_Plus(*dest_register, src_value, false);

    *dest_register += src_value;
 
    Set_Bit_N(&AF, N_FLAG, 0);
    return 1;
    }

    // SUB 8-Bit value from Register
    uint8_t CPU::CPU::SUB8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    Set_Half_Carry(h, -1 * src_value, true);

    Set_Carry_Minus(h, src_value);
    h -= src_value;
    if(!h)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
      
    reg = h << 8 | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    Set_Half_Carry(l, -1 * src_value, true);
      
    Set_Carry_Minus(l, src_value);

    l -= src_value;

    if(!l)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }

    reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(&AF, N_FLAG, 1);
    return 1;
    }

  
    // These ones only modify Register 'A' so I could remove the general part
    // Bitwise AND Register with value
    uint8_t CPU::CPU::AND8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    h &= src_value;
    if(!h)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = h << 8 | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    l &= src_value;
    if(!l)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 1);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 1;
    }

    // Bitwise OR on Register with value
    uint8_t CPU::CPU::OR8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    h |= src_value;
    if(!h)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = h << 8 | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    l |= src_value;
    if(!l)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 1;
    }

    // Bitwise XOR on Register with value
    // keep exception at 0xAF in mind
    uint8_t CPU::CPU::XOR8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t reg = *dest_register;
    if(higher_half)
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    h ^= src_value;
    if(!h)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = h << 8 | l;
    }
    else
    {
    uint8_t CPU::h = reg >> 8;
    uint8_t CPU::l = reg;

    l ^= src_value;
    if(!l)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    reg = (h << 8) | l;
    }
    *dest_register = reg;

    Set_Bit_N(&AF, N_FLAG, 0);
    Set_Bit_N(&AF, H_FLAG, 0);
    Set_Bit_N(&AF, C_FLAG, 0);
    return 1;
    }

  
    // Custom operation on Register with 8-Bit value
    // keep exception at 0x9F in mind
    uint8_t CPU::CPU::SBC8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t val = src_value - Get_Bit_N(AF, 4);
    Set_Half_Carry(*dest_register, -1 * val, true);
    Set_Carry_Minus(*dest_register, val);
    
    *dest_register -= val;

    if(*dest_register == 0)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    
    Set_Bit_N(&AF, N_FLAG, 1);

    return 1;
    }

    // Custom operation on Register with 8-Bit value
    uint8_t CPU::CPU::ADC8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t val = src_value + Get_Bit_N(AF, C_FLAG);
    
    Set_Half_Carry(*dest_register, val, true);
    Set_Carry_Plus(*dest_register, val, true);
    *dest_register += val;

    if(*dest_register == 0)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
     
    Set_Bit_N(&AF, N_FLAG, 0);

    return 1;
    }

    // Custom operation on Register with 8-Bit value
    // keep exception at 0xBF in mind
  
    uint8_t CPU::CPU::CP8(uint16_t *dest_register, uint16_t src_value, bool higher_half)
    {
    uint16_t cc = AF >> 8;
    uint16_t sub = src_value + Get_Bit_N(AF, C_FLAG);

    Set_Half_Carry(cc, -1 * sub, true);
    Set_Carry_Minus(cc, sub);
    cc -= src_value;
    
    if(!cc)
    {
    Set_Bit_N(&AF, Z_FLAG, 1);
    }
    Set_Bit_N(&AF, N_FLAG, 1);

    return 1;
    }
  */

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
      val = (*src_value >> 8) << 8;
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
      val = *src_value + bus->Read(PC);
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
      *dest_register &= 0x0F;
      *dest_register |= val;
    }

    if(w == IMode::LOW)
    {
      *dest_register &= 0xF0;
      *dest_register |= val;
    }

    if(w == IMode::ALL)
    {
      *dest_register = val;
    }

    if(w == IMode::MEM)
    {
      *dest_register = val;
    }

    if(w == IMode::MEMI)
    {
      *dest_register = val;
      *dest_register += 1;
    }

    if(w == IMode::MEMD)
    {
      *dest_register = val;
      *dest_register -= 1;
    }

    if(w == IMode::A16)
    {
      PC += 1;
      uint16_t addr = Combine(bus->Read(PC+1), bus->Read(PC));
      PC += 1;

      if(r == IMode::HIGH)
      {
	val >>= 8;
      }
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

      *dest_register &= 0x0F;
      *dest_register |= (val << 8);
    }

    return 0;
  }
  
  uint8_t CPU::LDH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::INC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::DEC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::ADD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::SUB(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::AND(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::OR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
// keep exception at 0xAF in mind
  uint8_t CPU::XOR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::POP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::PUSH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::JP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::JR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::CALL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

  uint8_t CPU::RST(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
  uint8_t CPU::RET(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

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

  }

  uint8_t CPU::ADC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }

// keep exception at 0xBF in mind
  uint8_t CPU::CP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r)
  {

  }
};
