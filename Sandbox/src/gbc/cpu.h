#pragma once
#include "Sandbox.h"
#include "gbc/bus.h"

namespace GBC
{

  // toggles higher_half, lower_half and 16-bit mode in opfunctions
  enum IMode
  {
    NONE=0x00, LOW, HIGH, ALL, MEM, N8, N16, A8, A16, E8
  };

  enum Jump
  {
    Z, NZ, C, NC
  };

  struct Instruction
  {
    std::string mnemonic;

    uint8_t cycle;

    uint8_t (*opfun)(uint16_t*, IMode, uint16_t*, IMode);
    IMode w;
    uint16_t *dest;

    IMode r;
    uint16_t *src;
  };

  enum State
  {
    STOP, RUN, HALT
  };
   
  struct CPU
  {
    uint16_t AF = 0; // Accumulator and Flags

    // switchable registers
    uint16_t BC = 0;
    uint16_t DE = 0;
    uint16_t HL = 0;

    uint16_t SP = 0; // Stack Pointer
    uint16_t PC = 0; // Program Counter
 
    State state;

    void Validate_Opcode(Bus *bus);

    Instruction instructions[256] = 
    {
      {"NOP", 4, &NOP, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x00
      {"LD BC, n16", 12, &LD, IMode::ALL, &BC, IMode::N16, nullptr}, //0x01
      {"LD BC, A", 8, &LD, IMode::MEM, &BC, IMode::HIGH, &AF}, //0x02
      {"INC BC", 8, &INC, IMode::ALL, &BC, IMode::NONE, nullptr}, //0x03
      {"INC B", 4, &INC, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x04
      {"DEC B", 4, &DEC, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x05
      {"LD B, n8", 8, &LD, IMode::HIGH, &BC, IMode::N8, nullptr}, //0x06
      {"RLCA", 4, &RLCA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x07
      {"LD a16, SP", 20, &LD, IMode::MEM, nullptr, IMode::ALL, &SP}, //0x08
      {"ADD HL, BC", 8, &ADD, IMode::ALL, &HL, IMode::ALL, &BC}, //0x09
      {"LD A, BC", 8, &LD, IMode::HIGH, &AF, IMode::MEM, &BC}, //0x0A
      {"DEC BC", 8, &DEC, IMode::ALL, &BC, IMode::NONE, nullptr}, //0x0B
      {"INC C", 4, &INC, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x0C
      {"DEC C", 4, &DEC, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x0D
      {"LD C, n8", 8, &LD, IMode::LOW, &BC, IMode::N8, nullptr}, //0x0E
      {"RRCA", 4, &RRCA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x0F
      {"STOP n8", 4, &STOP, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x10
      {"LD DE, n16", 12, &LD, IMode::ALL, &DE, IMode::N16, nullptr}, //0x11
      {"LD DE, A", 8, &LD, IMode::MEM, &DE, IMode::HIGH, &AF}, //0x12
      {"INC DE", 8, &INC, IMode::ALL, &DE, IMode::NONE, nullptr}, //0x13
      {"INC D", 4, &INC, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x14
      {"DEC D", 4, &DEC, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x15
      {"LD D, n8", 8, &LD, IMode::HIGH, &DE, IMode::N8, nullptr}, //0x16
      {"RLA", 4, &RLA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x17
      {"JR e8", 12, &JR, IMode::E8, nullptr, IMode::NONE, nullptr}, //0x18
      {"ADD HL, DE", 8, &ADD, IMode::ALL, &HL, IMode::ALL, &DE}, //0x19
      {"LD A, DE", 8, &LD, IMode::HIGH, &AF, IMode::MEM, &DE}, //0x1A
      {"DEC DE", 8, &DEC, IMode::ALL, &DE, IMode::NONE, nullptr}, //0x1B
      {"INC E", 4, &INC, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x1C
      {"DEC E", 4, &DEC, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x1D
      {"LD E, n8", 8, &LD, IMode::LOW, &DE, IMode::N8, nullptr}, //0x1E
      {"RRA", 4, &RRA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x1F
      {"JR NZ, e8", 12, &JR, IMode::NONE, (uint16_t*)Jump::NZ, IMode::E8, nullptr}, //0x20
      {"LD HL, n16", 12, &LD, IMode::ALL, &HL, IMode::N16, nullptr}, //0x21
      {"LD HL, A", 8, &LD, IMode::MEM, &HL, IMode::HIGH, &AF}, //0x22
      {"INC HL", 8, &INC, IMode::ALL, &HL, IMode::NONE, nullptr}, //0x23
      {"INC H", 4, &INC, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x24
      {"DEC H", 4, &DEC, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x25
      {"LD H, n8", 8, &LD, IMode::HIGH, &HL, IMode::N8, nullptr}, //0x26
      {"DAA", 4, &DAA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x27
      {"JR Z, e8", 12, &JR, IMode::NONE, (uint16_t*)Jump::Z, IMode::E8, nullptr}, //0x28
      {"ADD HL, HL", 8, &ADD, IMode::ALL, &HL, IMode::ALL, &HL}, //0x29
      {"LD A, HL", 8, &LD, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x2A
      {"DEC HL", 8, &DEC, IMode::ALL, &HL, IMode::NONE, nullptr}, //0x2B
      {"INC L", 4, &INC, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x2C
      {"DEC L", 4, &DEC, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x2D
      {"LD L, n8", 8, &LD, IMode::LOW, &HL, IMode::N8, nullptr}, //0x2E
      {"CPL", 4, &CPL, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x2F
      {"JR NC, e8", 12, &JR, IMode::NONE, (uint16_t*)Jump::NC, IMode::E8, nullptr}, //0x30
      {"LD SP, n16", 12, &LD, IMode::ALL, &SP, IMode::N16, nullptr}, //0x31
      {"LD HL, A", 8, &LD, IMode::MEM, &HL, IMode::HIGH, &AF}, //0x32
      {"INC SP", 8, &INC, IMode::ALL, &SP, IMode::NONE, nullptr}, //0x33
      {"INC HL", 12, &INC, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x34
      {"DEC HL", 12, &DEC, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x35
      {"LD HL, n8", 12, &LD, IMode::MEM, &HL, IMode::N8, nullptr}, //0x36
      {"SCF", 4, &SCF, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x37
      {"JR C, e8", 12, &JR, IMode::NONE, (uint16_t*)Jump::C, IMode::E8, nullptr}, //0x38
      {"ADD HL, SP", 8, &ADD, IMode::ALL, &HL, IMode::ALL, &SP}, //0x39
      {"LD A, HL", 8, &LD, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x3A
      {"DEC SP", 8, &DEC, IMode::ALL, &SP, IMode::NONE, nullptr}, //0x3B
      {"INC A", 4, &INC, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x3C
      {"DEC A", 4, &DEC, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x3D
      {"LD A, n8", 8, &LD, IMode::HIGH, &AF, IMode::N8, nullptr}, //0x3E
      {"CCF", 4, &CCF, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x3F
      {"LD B, B", 4, &LD, IMode::HIGH, &BC, IMode::HIGH, &BC}, //0x40
      {"LD B, C", 4, &LD, IMode::HIGH, &BC, IMode::LOW, &BC}, //0x41
      {"LD B, D", 4, &LD, IMode::HIGH, &BC, IMode::HIGH, &DE}, //0x42
      {"LD B, E", 4, &LD, IMode::HIGH, &BC, IMode::LOW, &DE}, //0x43
      {"LD B, H", 4, &LD, IMode::HIGH, &BC, IMode::HIGH, &HL}, //0x44
      {"LD B, L", 4, &LD, IMode::HIGH, &BC, IMode::LOW, &HL}, //0x45
      {"LD B, HL", 8, &LD, IMode::HIGH, &BC, IMode::MEM, &HL}, //0x46
      {"LD B, A", 4, &LD, IMode::HIGH, &BC, IMode::HIGH, &AF}, //0x47
      {"LD C, B", 4, &LD, IMode::LOW, &BC, IMode::HIGH, &BC}, //0x48
      {"LD C, C", 4, &LD, IMode::LOW, &BC, IMode::LOW, &BC}, //0x49
      {"LD C, D", 4, &LD, IMode::LOW, &BC, IMode::HIGH, &DE}, //0x4A
      {"LD C, E", 4, &LD, IMode::LOW, &BC, IMode::LOW, &DE}, //0x4B
      {"LD C, H", 4, &LD, IMode::LOW, &BC, IMode::HIGH, &HL}, //0x4C
      {"LD C, L", 4, &LD, IMode::LOW, &BC, IMode::LOW, &HL}, //0x4D
      {"LD C, HL", 8, &LD, IMode::LOW, &BC, IMode::MEM, &HL}, //0x4E
      {"LD C, A", 4, &LD, IMode::LOW, &BC, IMode::HIGH, &AF}, //0x4F
      {"LD D, B", 4, &LD, IMode::HIGH, &DE, IMode::HIGH, &BC}, //0x50
      {"LD D, C", 4, &LD, IMode::HIGH, &DE, IMode::LOW, &BC}, //0x51
      {"LD D, D", 4, &LD, IMode::HIGH, &DE, IMode::HIGH, &DE}, //0x52
      {"LD D, E", 4, &LD, IMode::HIGH, &DE, IMode::LOW, &DE}, //0x53
      {"LD D, H", 4, &LD, IMode::HIGH, &DE, IMode::HIGH, &HL}, //0x54
      {"LD D, L", 4, &LD, IMode::HIGH, &DE, IMode::LOW, &HL}, //0x55
      {"LD D, HL", 8, &LD, IMode::HIGH, &DE, IMode::MEM, &HL}, //0x56
      {"LD D, A", 4, &LD, IMode::HIGH, &DE, IMode::HIGH, &AF}, //0x57
      {"LD E, B", 4, &LD, IMode::LOW, &DE, IMode::HIGH, &BC}, //0x58
      {"LD E, C", 4, &LD, IMode::LOW, &DE, IMode::LOW, &BC}, //0x59
      {"LD E, D", 4, &LD, IMode::LOW, &DE, IMode::HIGH, &DE}, //0x5A
      {"LD E, E", 4, &LD, IMode::LOW, &DE, IMode::LOW, &DE}, //0x5B
      {"LD E, H", 4, &LD, IMode::LOW, &DE, IMode::HIGH, &HL}, //0x5C
      {"LD E, L", 4, &LD, IMode::LOW, &DE, IMode::LOW, &HL}, //0x5D
      {"LD E, HL", 8, &LD, IMode::LOW, &DE, IMode::MEM, &HL}, //0x5E
      {"LD E, A", 4, &LD, IMode::LOW, &DE, IMode::HIGH, &AF}, //0x5F
      {"LD H, B", 4, &LD, IMode::HIGH, &HL, IMode::HIGH, &BC}, //0x60
      {"LD H, C", 4, &LD, IMode::HIGH, &HL, IMode::LOW, &BC}, //0x61
      {"LD H, D", 4, &LD, IMode::HIGH, &HL, IMode::HIGH, &DE}, //0x62
      {"LD H, E", 4, &LD, IMode::HIGH, &HL, IMode::LOW, &DE}, //0x63
      {"LD H, H", 4, &LD, IMode::HIGH, &HL, IMode::HIGH, &HL}, //0x64
      {"LD H, L", 4, &LD, IMode::HIGH, &HL, IMode::LOW, &HL}, //0x65
      {"LD H, HL", 8, &LD, IMode::HIGH, &HL, IMode::MEM, &HL}, //0x66
      {"LD H, A", 4, &LD, IMode::HIGH, &HL, IMode::HIGH, &AF}, //0x67
      {"LD L, B", 4, &LD, IMode::LOW, &HL, IMode::HIGH, &BC}, //0x68
      {"LD L, C", 4, &LD, IMode::LOW, &HL, IMode::LOW, &BC}, //0x69
      {"LD L, D", 4, &LD, IMode::LOW, &HL, IMode::HIGH, &DE}, //0x6A
      {"LD L, E", 4, &LD, IMode::LOW, &HL, IMode::LOW, &DE}, //0x6B
      {"LD L, H", 4, &LD, IMode::LOW, &HL, IMode::HIGH, &HL}, //0x6C
      {"LD L, L", 4, &LD, IMode::LOW, &HL, IMode::LOW, &HL}, //0x6D
      {"LD L, HL", 8, &LD, IMode::LOW, &HL, IMode::MEM, &HL}, //0x6E
      {"LD L, A", 4, &LD, IMode::LOW, &HL, IMode::HIGH, &AF}, //0x6F
      {"LD HL, B", 8, &LD, IMode::MEM, &HL, IMode::HIGH, &BC}, //0x70
      {"LD HL, C", 8, &LD, IMode::MEM, &HL, IMode::LOW, &BC}, //0x71
      {"LD HL, D", 8, &LD, IMode::MEM, &HL, IMode::HIGH, &DE}, //0x72
      {"LD HL, E", 8, &LD, IMode::MEM, &HL, IMode::LOW, &DE}, //0x73
      {"LD HL, H", 8, &LD, IMode::MEM, &HL, IMode::HIGH, &HL}, //0x74
      {"LD HL, L", 8, &LD, IMode::MEM, &HL, IMode::LOW, &HL}, //0x75
      {"HALT", 4, &HALT, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x76
      {"LD HL, A", 8, &LD, IMode::MEM, &HL, IMode::HIGH, &AF}, //0x77
      {"LD A, B", 4, &LD, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x78
      {"LD A, C", 4, &LD, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x79
      {"LD A, D", 4, &LD, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x7A
      {"LD A, E", 4, &LD, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x7B
      {"LD A, H", 4, &LD, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x7C
      {"LD A, L", 4, &LD, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x7D
      {"LD A, HL", 8, &LD, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x7E
      {"LD A, A", 4, &LD, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x7F
      {"ADD A, B", 4, &ADD, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x80
      {"ADD A, C", 4, &ADD, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x81
      {"ADD A, D", 4, &ADD, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x82
      {"ADD A, E", 4, &ADD, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x83
      {"ADD A, H", 4, &ADD, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x84
      {"ADD A, L", 4, &ADD, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x85
      {"ADD A, HL", 8, &ADD, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x86
      {"ADD A, A", 4, &ADD, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x87
      {"ADC A, B", 4, &ADC, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x88
      {"ADC A, C", 4, &ADC, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x89
      {"ADC A, D", 4, &ADC, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x8A
      {"ADC A, E", 4, &ADC, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x8B
      {"ADC A, H", 4, &ADC, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x8C
      {"ADC A, L", 4, &ADC, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x8D
      {"ADC A, HL", 8, &ADC, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x8E
      {"ADC A, A", 4, &ADC, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x8F
      {"SUB A, B", 4, &SUB, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x90
      {"SUB A, C", 4, &SUB, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x91
      {"SUB A, D", 4, &SUB, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x92
      {"SUB A, E", 4, &SUB, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x93
      {"SUB A, H", 4, &SUB, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x94
      {"SUB A, L", 4, &SUB, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x95
      {"SUB A, HL", 8, &SUB, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x96
      {"SUB A, A", 4, &SUB, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x97
      {"SBC A, B", 4, &SBC, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x98
      {"SBC A, C", 4, &SBC, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x99
      {"SBC A, D", 4, &SBC, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x9A
      {"SBC A, E", 4, &SBC, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x9B
      {"SBC A, H", 4, &SBC, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x9C
      {"SBC A, L", 4, &SBC, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x9D
      {"SBC A, HL", 8, &SBC, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x9E
      {"SBC A, A", 4, &SBC, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x9F
      {"AND A, B", 4, &AND, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xA0
      {"AND A, C", 4, &AND, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xA1
      {"AND A, D", 4, &AND, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xA2
      {"AND A, E", 4, &AND, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xA3
      {"AND A, H", 4, &AND, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xA4
      {"AND A, L", 4, &AND, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xA5
      {"AND A, HL", 8, &AND, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xA6
      {"AND A, A", 4, &AND, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xA7
      {"XOR A, B", 4, &XOR, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xA8
      {"XOR A, C", 4, &XOR, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xA9
      {"XOR A, D", 4, &XOR, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xAA
      {"XOR A, E", 4, &XOR, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xAB
      {"XOR A, H", 4, &XOR, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xAC
      {"XOR A, L", 4, &XOR, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xAD
      {"XOR A, HL", 8, &XOR, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xAE
      {"XOR A, A", 4, &XOR, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xAF
      {"OR A, B", 4, &OR, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xB0
      {"OR A, C", 4, &OR, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xB1
      {"OR A, D", 4, &OR, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xB2
      {"OR A, E", 4, &OR, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xB3
      {"OR A, H", 4, &OR, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xB4
      {"OR A, L", 4, &OR, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xB5
      {"OR A, HL", 8, &OR, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xB6
      {"OR A, A", 4, &OR, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xB7
      {"CP A, B", 4, &CP, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xB8
      {"CP A, C", 4, &CP, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xB9
      {"CP A, D", 4, &CP, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xBA
      {"CP A, E", 4, &CP, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xBB
      {"CP A, H", 4, &CP, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xBC
      {"CP A, L", 4, &CP, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xBD
      {"CP A, HL", 8, &CP, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xBE
      {"CP A, A", 4, &CP, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xBF
      {"RET NZ", 20, &RET, IMode::NONE, (uint16_t*)Jump::NZ, IMode::NONE, nullptr}, //0xC0
      {"POP BC", 12, &POP, IMode::NONE, &BC, IMode::NONE, nullptr}, //0xC1
      {"JP NZ, a16", 16, &JP, IMode::NONE, (uint16_t*)Jump::NZ, IMode::A16, nullptr}, //0xC2
      {"JP a16", 16, &JP, IMode::A16, nullptr, IMode::NONE, nullptr}, //0xC3
      {"CALL NZ, a16", 24, &CALL, IMode::NONE, (uint16_t*)Jump::NZ, IMode::A16, nullptr}, //0xC4
      {"PUSH BC", 16, &PUSH, IMode::NONE, &BC, IMode::NONE, nullptr}, //0xC5
      {"ADD A, n8", 8, &ADD, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xC6
      {"RST $00", 16, &RST, IMode::NONE, 0, IMode::NONE, nullptr}, //0xC7
      {"RET Z", 20, &RET, IMode::NONE, (uint16_t*)Jump::Z, IMode::NONE, nullptr}, //0xC8
      {"RET", 16, &RET, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xC9
      {"JP Z, a16", 16, &JP, IMode::NONE, (uint16_t*)Jump::Z, IMode::A16, nullptr}, //0xCA
      {"PREFIX", 4, &PREFIX, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xCB
      {"CALL Z, a16", 24, &CALL, IMode::NONE, (uint16_t*)Jump::Z, IMode::A16, nullptr}, //0xCC
      {"CALL a16", 24, &CALL, IMode::A16, nullptr, IMode::NONE, nullptr}, //0xCD
      {"ADC A, n8", 8, &ADC, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xCE
      {"RST $08", 16, &RST, IMode::NONE, (uint16_t*)8, IMode::NONE, nullptr}, //0xCF
      {"RET NC", 20, &RET, IMode::NONE, (uint16_t*)Jump::NC, IMode::NONE, nullptr}, //0xD0
      {"POP DE", 12, &POP, IMode::NONE, &DE, IMode::NONE, nullptr}, //0xD1
      {"JP NC, a16", 16, &JP, IMode::NONE, (uint16_t*)Jump::NC, IMode::A16, nullptr}, //0xD2
      {"ILLEGAL_D3", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xD3
      {"CALL NC, a16", 24, &CALL, IMode::NONE, (uint16_t*)Jump::NC, IMode::A16, nullptr}, //0xD4
      {"PUSH DE", 16, &PUSH, IMode::NONE, &DE, IMode::NONE, nullptr}, //0xD5
      {"SUB A, n8", 8, &SUB, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xD6
      {"RST $10", 16, &RST, IMode::NONE, (uint16_t*)10, IMode::NONE, nullptr}, //0xD7
      {"RET C", 20, &RET, IMode::NONE, (uint16_t*)Jump::C, IMode::NONE, nullptr}, //0xD8
      {"RETI", 16, &RETI, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xD9
      {"JP C, a16", 16, &JP, IMode::NONE, (uint16_t*)Jump::C, IMode::A16, nullptr}, //0xDA
      {"ILLEGAL_DB", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xDB
      {"CALL C, a16", 24, &CALL, IMode::NONE, (uint16_t*)Jump::C, IMode::A16, nullptr}, //0xDC
      {"ILLEGAL_DD", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xDD
      {"SBC A, n8", 8, &SBC, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xDE
      {"RST $18", 16, &RST, IMode::NONE, (uint16_t*)18, IMode::NONE, nullptr}, //0xDF
      {"LDH a8, A", 12, &LDH, IMode::A8, nullptr, IMode::HIGH, &AF}, //0xE0
      {"POP HL", 12, &POP, IMode::NONE, &HL, IMode::NONE, nullptr}, //0xE1
      {"LD C, A", 8, &LDS, IMode::MEM, &BC, IMode::HIGH, &AF}, //0xE2
      {"ILLEGAL_E3", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xE3
      {"ILLEGAL_E4", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xE4
      {"PUSH HL", 16, &PUSH, IMode::NONE, &HL, IMode::NONE, nullptr}, //0xE5
      {"AND A, n8", 8, &AND, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xE6
      {"RST $20", 16, &RST, IMode::NONE, (uint16_t*)20, IMode::NONE, nullptr}, //0xE7
      {"ADD SP, e8", 16, &ADD, IMode::ALL, &SP, IMode::E8, nullptr}, //0xE8
      {"JP HL", 4, &JP, IMode::ALL, &HL, IMode::NONE, nullptr}, //0xE9
      {"LD a16, A", 16, &LD, IMode::MEM, nullptr, IMode::HIGH, &AF}, //0xEA
      {"ILLEGAL_EB", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xEB
      {"ILLEGAL_EC", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xEC
      {"ILLEGAL_ED", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xED
      {"XOR A, n8", 8, &XOR, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xEE
      {"RST $28", 16, &RST, IMode::NONE, (uint16_t*)28, IMode::NONE, nullptr}, //0xEF
      {"LDH A, a8", 12, &LDH, IMode::HIGH, &AF, IMode::A8, nullptr}, //0xF0
      {"POP AF", 12, &POP, IMode::NONE, &AF, IMode::NONE, nullptr}, //0xF1
      {"LD A, C", 8, &LDS, IMode::HIGH, &AF, IMode::MEM, &BC}, //0xF2
      {"DI", 4, &DI, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xF3
      {"ILLEGAL_F4", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xF4
      {"PUSH AF", 16, &PUSH, IMode::NONE, &AF, IMode::NONE, nullptr}, //0xF5
      {"OR A, n8", 8, &OR, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xF6
      {"RST $30", 16, &RST, IMode::NONE, (uint16_t*)30, IMode::NONE, nullptr}, //0xF7
      {"LD HL, SP", 12, &LD, IMode::ALL, &HL, IMode::ALL, &SP}, //0xF8
      {"LD SP, HL", 8, &LD, IMode::ALL, &SP, IMode::ALL, &HL}, //0xF9
      {"LD A, a16", 16, &LD, IMode::HIGH, &AF, IMode::MEM, nullptr}, //0xFA
      {"EI", 4, &EI, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xFB
      {"ILLEGAL_FC", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xFC
      {"ILLEGAL_FD", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xFD
      {"CP A, n8", 8, &CP, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xFE
      {"RST $38", 16, &RST, IMode::NONE, (uint16_t*)38, IMode::NONE, nullptr}, //0xFF
    };
  private:
    void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
    uint16_t Get_Bit_N(uint16_t src, uint8_t n);
  
    void Set_Half_Carry(uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Plus(uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Minus(uint16_t src_register, uint16_t val);
  
    static inline uint8_t NOP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){}
    static inline uint8_t HALT(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){}
    static inline uint8_t STOP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){}

    // 0xCB devil
    static inline uint8_t PREFIX(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){}
    
    static uint8_t EI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t DI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t LD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t LDS(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t LDH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t INC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t DEC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t ADD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t SUB(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t AND(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t OR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    // keep exception at 0xAF in mind
    static uint8_t XOR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t POP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t PUSH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t JP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t JR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t CALL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t RST(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t RET(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t RETI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t CPL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t CCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t DAA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t SCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t RRCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t RRA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t RLCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    static uint8_t RLA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    
    // keep exception at 0x9F in mind
    static uint8_t SBC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t ADC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    // keep exception at 0xBF in mind
    static uint8_t CP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
  };

};
