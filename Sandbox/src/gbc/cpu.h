#pragma once
#include "Sandbox.h"
#include "gbc/bus.h"

namespace GBC
{

  // toggles higher_half, lower_half and 16-bit mode in opfunctions
  enum IMode
    {
      NONE=0, LOW, HIGH, ALL, MEM, MEMI, MEMD, N8, N16, A8, A16, E8, Z, NZ, C, NC, R10, R20, R30, R8, R18, R28, R38
    };

  enum State
    {
      STOP, RUN, HALT
    };
   
  struct CPU
  {
    inline CPU(Bus *b) { this->bus = b; }
    uint16_t AF = 0; // Accumulator and Flags

    // switchable registers
    uint16_t BC = 0;
    uint16_t DE = 0;
    uint16_t HL = 0;

    uint16_t SP = 0; // Stack Pointer
    uint16_t PC = 0; // Program Counter
 
    State state;

    Bus *bus;
    
    void Validate_Opcode();

    inline uint8_t NOP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){ return 0; }
    inline uint8_t HALT(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){ state = State::HALT; return 0; }
    inline uint8_t STOP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){ state = State::STOP; return 0; }

    // 0xCB devil
    uint8_t PREFIX(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    
    uint8_t EI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t DI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t LD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t LDS(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t LDH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t INC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t DEC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t ADD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SUB(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t AND(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t OR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    // keep exception at 0xAF in mind
    uint8_t XOR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t POP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t PUSH(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t JP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t JR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t CALL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t RST(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RET(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RETI(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t CPL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t CCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t DAA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SCF(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t RRCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RRA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RLCA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RLA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    
    // keep exception at 0x9F in mind
    uint8_t SBC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    uint8_t ADC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    // keep exception at 0xBF in mind
    uint8_t CP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    // 0xcb prefixed instructions
    uint8_t RES(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SET(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t IBIT(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SWAP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SLA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SRA(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RRC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t SRL(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
    uint8_t RLC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

  private:
    void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
    uint16_t Get_Bit_N(uint16_t src, uint8_t n);
  
    void Set_Half_Carry(uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Plus(uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Minus(uint16_t src_register, uint8_t val);

    struct Instruction
    {
      const char* mnemonic;

      uint8_t cycle;

      uint8_t (CPU::*opfun)(uint16_t*, IMode, uint16_t*, IMode);
      IMode w;
      uint16_t *dest;

      IMode r;
      uint16_t *src;
    };
  public:
    Instruction lookup[256] =
    {
      {"NOP", 4, &CPU::NOP, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x00
      {"LD BC, n16", 12, &CPU::LD, IMode::ALL, &BC, IMode::N16, nullptr}, //0x01
      {"LD BC, A", 8, &CPU::LD, IMode::MEM, &BC, IMode::HIGH, &AF}, //0x02
      {"INC BC", 8, &CPU::INC, IMode::ALL, &BC, IMode::NONE, 0}, //0x03
      {"INC B", 4, &CPU::INC, IMode::HIGH, &BC, IMode::NONE, 0}, //0x04
      {"DEC B", 4, &CPU::DEC, IMode::HIGH, &BC, IMode::NONE, 0}, //0x05
      {"LD B, n8", 8, &CPU::LD, IMode::HIGH, &BC, IMode::N8, nullptr}, //0x06
      {"RLCA", 4, &CPU::RLCA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x07
      {"LD a16, SP", 20, &CPU::LD, IMode::A16, nullptr, IMode::ALL, &SP}, //0x08
      {"ADD HL, BC", 8, &CPU::ADD, IMode::ALL, &HL, IMode::ALL, &BC}, //0x09
      {"LD A, BC", 8, &CPU::LD, IMode::HIGH, &AF, IMode::MEM, &BC}, //0x0A
      {"DEC BC", 8, &CPU::DEC, IMode::ALL, &BC, IMode::NONE, 0}, //0x0B
      {"INC C", 4, &CPU::INC, IMode::LOW, &BC, IMode::NONE, 0}, //0x0C
      {"DEC C", 4, &CPU::DEC, IMode::LOW, &BC, IMode::NONE, 0}, //0x0D
      {"LD C, n8", 8, &CPU::LD, IMode::LOW, &BC, IMode::N8, nullptr}, //0x0E
      {"RRCA", 4, &CPU::RRCA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x0F
      {"STOP n8", 4, &CPU::STOP, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x10
      {"LD DE, n16", 12, &CPU::LD, IMode::ALL, &DE, IMode::N16, nullptr}, //0x11
      {"LD DE, A", 8, &CPU::LD, IMode::MEM, &DE, IMode::HIGH, &AF}, //0x12
      {"INC DE", 8, &CPU::INC, IMode::ALL, &DE, IMode::NONE, 0}, //0x13
      {"INC D", 4, &CPU::INC, IMode::HIGH, &DE, IMode::NONE, 0}, //0x14
      {"DEC D", 4, &CPU::DEC, IMode::HIGH, &DE, IMode::NONE, 0}, //0x15
      {"LD D, n8", 8, &CPU::LD, IMode::HIGH, &DE, IMode::N8, nullptr}, //0x16
      {"RLA", 4, &CPU::RLA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x17
      {"JR e8", 12, &CPU::JR, IMode::E8, nullptr, IMode::NONE, nullptr}, //0x18
      {"ADD HL, DE", 8, &CPU::ADD, IMode::ALL, &HL, IMode::ALL, &DE}, //0x19
      {"LD A, DE", 8, &CPU::LD, IMode::HIGH, &AF, IMode::MEM, &DE}, //0x1A
      {"DEC DE", 8, &CPU::DEC, IMode::ALL, &DE, IMode::NONE, 0}, //0x1B
      {"INC E", 4, &CPU::INC, IMode::LOW, &DE, IMode::NONE, 0}, //0x1C
      {"DEC E", 4, &CPU::DEC, IMode::LOW, &DE, IMode::NONE, 0}, //0x1D
      {"LD E, n8", 8, &CPU::LD, IMode::LOW, &DE, IMode::N8, nullptr}, //0x1E
      {"RRA", 4, &CPU::RRA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x1F
      {"JR NZ, e8", 12, &CPU::JR, IMode::NZ, nullptr, IMode::E8, nullptr}, //0x20
      {"LD HL, n16", 12, &CPU::LD, IMode::ALL, &HL, IMode::N16, nullptr}, //0x21
      {"LD HL+, A", 8, &CPU::LD, IMode::MEMI, &HL, IMode::HIGH, &AF}, //0x22
      {"INC HL", 8, &CPU::INC, IMode::ALL, &HL, IMode::NONE, 0}, //0x23
      {"INC H", 4, &CPU::INC, IMode::HIGH, &HL, IMode::NONE, 0}, //0x24
      {"DEC H", 4, &CPU::DEC, IMode::HIGH, &HL, IMode::NONE, 0}, //0x25
      {"LD H, n8", 8, &CPU::LD, IMode::HIGH, &HL, IMode::N8, nullptr}, //0x26
      {"DAA", 4, &CPU::DAA, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x27
      {"JR Z, e8", 12, &CPU::JR, IMode::Z, nullptr, IMode::E8, nullptr}, //0x28
      {"ADD HL, HL", 8, &CPU::ADD, IMode::ALL, &HL, IMode::ALL, &HL}, //0x29
      {"LD A, HL+", 8, &CPU::LD, IMode::HIGH, &AF, IMode::MEMI, &HL}, //0x2A
      {"DEC HL", 8, &CPU::DEC, IMode::ALL, &HL, IMode::NONE, 0}, //0x2B
      {"INC L", 4, &CPU::INC, IMode::LOW, &HL, IMode::NONE, 0}, //0x2C
      {"DEC L", 4, &CPU::DEC, IMode::LOW, &HL, IMode::NONE, 0}, //0x2D
      {"LD L, n8", 8, &CPU::LD, IMode::LOW, &HL, IMode::N8, nullptr}, //0x2E
      {"CPL", 4, &CPU::CPL, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x2F
      {"JR NC, e8", 12, &CPU::JR, IMode::NC, nullptr, IMode::E8, nullptr}, //0x30
      {"LD SP, n16", 12, &CPU::LD, IMode::ALL, &SP, IMode::N16, nullptr}, //0x31
      {"LD HL-, A", 8, &CPU::LD, IMode::MEMD, &HL, IMode::HIGH, &AF}, //0x32
      {"INC SP", 8, &CPU::INC, IMode::ALL, &SP, IMode::NONE, 0}, //0x33
      {"INC HL", 12, &CPU::INC, IMode::MEM, &HL, IMode::NONE, 0}, //0x34
      {"DEC HL", 12, &CPU::DEC, IMode::MEM, &HL, IMode::NONE, 0}, //0x35
      {"LD HL, n8", 12, &CPU::LD, IMode::MEM, &HL, IMode::N8, nullptr}, //0x36
      {"SCF", 4, &CPU::SCF, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x37
      {"JR C, e8", 12, &CPU::JR, IMode::C, nullptr, IMode::E8, nullptr}, //0x38
      {"ADD HL, SP", 8, &CPU::ADD, IMode::ALL, &HL, IMode::ALL, &SP}, //0x39
      {"LD A, HL-", 8, &CPU::LD, IMode::HIGH, &AF, IMode::MEMD, &HL}, //0x3A
      {"DEC SP", 8, &CPU::DEC, IMode::ALL, &SP, IMode::NONE, 0}, //0x3B
      {"INC A", 4, &CPU::INC, IMode::HIGH, &AF, IMode::NONE, 0}, //0x3C
      {"DEC A", 4, &CPU::DEC, IMode::HIGH, &AF, IMode::NONE, 0}, //0x3D
      {"LD A, n8", 8, &CPU::LD, IMode::HIGH, &AF, IMode::N8, nullptr}, //0x3E
      {"CCF", 4, &CPU::CCF, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x3F
      {"LD B, B", 4, &CPU::LD, IMode::HIGH, &BC, IMode::HIGH, &BC}, //0x40
      {"LD B, C", 4, &CPU::LD, IMode::HIGH, &BC, IMode::LOW, &BC}, //0x41
      {"LD B, D", 4, &CPU::LD, IMode::HIGH, &BC, IMode::HIGH, &DE}, //0x42
      {"LD B, E", 4, &CPU::LD, IMode::HIGH, &BC, IMode::LOW, &DE}, //0x43
      {"LD B, H", 4, &CPU::LD, IMode::HIGH, &BC, IMode::HIGH, &HL}, //0x44
      {"LD B, L", 4, &CPU::LD, IMode::HIGH, &BC, IMode::LOW, &HL}, //0x45
      {"LD B, HL", 8, &CPU::LD, IMode::HIGH, &BC, IMode::MEM, &HL}, //0x46
      {"LD B, A", 4, &CPU::LD, IMode::HIGH, &BC, IMode::HIGH, &AF}, //0x47
      {"LD C, B", 4, &CPU::LD, IMode::LOW, &BC, IMode::HIGH, &BC}, //0x48
      {"LD C, C", 4, &CPU::LD, IMode::LOW, &BC, IMode::LOW, &BC}, //0x49
      {"LD C, D", 4, &CPU::LD, IMode::LOW, &BC, IMode::HIGH, &DE}, //0x4A
      {"LD C, E", 4, &CPU::LD, IMode::LOW, &BC, IMode::LOW, &DE}, //0x4B
      {"LD C, H", 4, &CPU::LD, IMode::LOW, &BC, IMode::HIGH, &HL}, //0x4C
      {"LD C, L", 4, &CPU::LD, IMode::LOW, &BC, IMode::LOW, &HL}, //0x4D
      {"LD C, HL", 8, &CPU::LD, IMode::LOW, &BC, IMode::MEM, &HL}, //0x4E
      {"LD C, A", 4, &CPU::LD, IMode::LOW, &BC, IMode::HIGH, &AF}, //0x4F
      {"LD D, B", 4, &CPU::LD, IMode::HIGH, &DE, IMode::HIGH, &BC}, //0x50
      {"LD D, C", 4, &CPU::LD, IMode::HIGH, &DE, IMode::LOW, &BC}, //0x51
      {"LD D, D", 4, &CPU::LD, IMode::HIGH, &DE, IMode::HIGH, &DE}, //0x52
      {"LD D, E", 4, &CPU::LD, IMode::HIGH, &DE, IMode::LOW, &DE}, //0x53
      {"LD D, H", 4, &CPU::LD, IMode::HIGH, &DE, IMode::HIGH, &HL}, //0x54
      {"LD D, L", 4, &CPU::LD, IMode::HIGH, &DE, IMode::LOW, &HL}, //0x55
      {"LD D, HL", 8, &CPU::LD, IMode::HIGH, &DE, IMode::MEM, &HL}, //0x56
      {"LD D, A", 4, &CPU::LD, IMode::HIGH, &DE, IMode::HIGH, &AF}, //0x57
      {"LD E, B", 4, &CPU::LD, IMode::LOW, &DE, IMode::HIGH, &BC}, //0x58
      {"LD E, C", 4, &CPU::LD, IMode::LOW, &DE, IMode::LOW, &BC}, //0x59
      {"LD E, D", 4, &CPU::LD, IMode::LOW, &DE, IMode::HIGH, &DE}, //0x5A
      {"LD E, E", 4, &CPU::LD, IMode::LOW, &DE, IMode::LOW, &DE}, //0x5B
      {"LD E, H", 4, &CPU::LD, IMode::LOW, &DE, IMode::HIGH, &HL}, //0x5C
      {"LD E, L", 4, &CPU::LD, IMode::LOW, &DE, IMode::LOW, &HL}, //0x5D
      {"LD E, HL", 8, &CPU::LD, IMode::LOW, &DE, IMode::MEM, &HL}, //0x5E
      {"LD E, A", 4, &CPU::LD, IMode::LOW, &DE, IMode::HIGH, &AF}, //0x5F
      {"LD H, B", 4, &CPU::LD, IMode::HIGH, &HL, IMode::HIGH, &BC}, //0x60
      {"LD H, C", 4, &CPU::LD, IMode::HIGH, &HL, IMode::LOW, &BC}, //0x61
      {"LD H, D", 4, &CPU::LD, IMode::HIGH, &HL, IMode::HIGH, &DE}, //0x62
      {"LD H, E", 4, &CPU::LD, IMode::HIGH, &HL, IMode::LOW, &DE}, //0x63
      {"LD H, H", 4, &CPU::LD, IMode::HIGH, &HL, IMode::HIGH, &HL}, //0x64
      {"LD H, L", 4, &CPU::LD, IMode::HIGH, &HL, IMode::LOW, &HL}, //0x65
      {"LD H, HL", 8, &CPU::LD, IMode::HIGH, &HL, IMode::MEM, &HL}, //0x66
      {"LD H, A", 4, &CPU::LD, IMode::HIGH, &HL, IMode::HIGH, &AF}, //0x67
      {"LD L, B", 4, &CPU::LD, IMode::LOW, &HL, IMode::HIGH, &BC}, //0x68
      {"LD L, C", 4, &CPU::LD, IMode::LOW, &HL, IMode::LOW, &BC}, //0x69
      {"LD L, D", 4, &CPU::LD, IMode::LOW, &HL, IMode::HIGH, &DE}, //0x6A
      {"LD L, E", 4, &CPU::LD, IMode::LOW, &HL, IMode::LOW, &DE}, //0x6B
      {"LD L, H", 4, &CPU::LD, IMode::LOW, &HL, IMode::HIGH, &HL}, //0x6C
      {"LD L, L", 4, &CPU::LD, IMode::LOW, &HL, IMode::LOW, &HL}, //0x6D
      {"LD L, HL", 8, &CPU::LD, IMode::LOW, &HL, IMode::MEM, &HL}, //0x6E
      {"LD L, A", 4, &CPU::LD, IMode::LOW, &HL, IMode::HIGH, &AF}, //0x6F
      {"LD HL, B", 8, &CPU::LD, IMode::MEM, &HL, IMode::HIGH, &BC}, //0x70
      {"LD HL, C", 8, &CPU::LD, IMode::MEM, &HL, IMode::LOW, &BC}, //0x71
      {"LD HL, D", 8, &CPU::LD, IMode::MEM, &HL, IMode::HIGH, &DE}, //0x72
      {"LD HL, E", 8, &CPU::LD, IMode::MEM, &HL, IMode::LOW, &DE}, //0x73
      {"LD HL, H", 8, &CPU::LD, IMode::MEM, &HL, IMode::HIGH, &HL}, //0x74
      {"LD HL, L", 8, &CPU::LD, IMode::MEM, &HL, IMode::LOW, &HL}, //0x75
      {"HALT", 4, &CPU::HALT, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0x76
      {"LD HL, A", 8, &CPU::LD, IMode::MEM, &HL, IMode::HIGH, &AF}, //0x77
      {"LD A, B", 4, &CPU::LD, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x78
      {"LD A, C", 4, &CPU::LD, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x79
      {"LD A, D", 4, &CPU::LD, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x7A
      {"LD A, E", 4, &CPU::LD, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x7B
      {"LD A, H", 4, &CPU::LD, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x7C
      {"LD A, L", 4, &CPU::LD, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x7D
      {"LD A, HL", 8, &CPU::LD, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x7E
      {"LD A, A", 4, &CPU::LD, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x7F
      {"ADD A, B", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x80
      {"ADD A, C", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x81
      {"ADD A, D", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x82
      {"ADD A, E", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x83
      {"ADD A, H", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x84
      {"ADD A, L", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x85
      {"ADD A, HL", 8, &CPU::ADD, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x86
      {"ADD A, A", 4, &CPU::ADD, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x87
      {"ADC A, B", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x88
      {"ADC A, C", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x89
      {"ADC A, D", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x8A
      {"ADC A, E", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x8B
      {"ADC A, H", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x8C
      {"ADC A, L", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x8D
      {"ADC A, HL", 8, &CPU::ADC, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x8E
      {"ADC A, A", 4, &CPU::ADC, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x8F
      {"SUB A, B", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x90
      {"SUB A, C", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x91
      {"SUB A, D", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x92
      {"SUB A, E", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x93
      {"SUB A, H", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x94
      {"SUB A, L", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x95
      {"SUB A, HL", 8, &CPU::SUB, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x96
      {"SUB A, A", 4, &CPU::SUB, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x97
      {"SBC A, B", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0x98
      {"SBC A, C", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::LOW, &BC}, //0x99
      {"SBC A, D", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0x9A
      {"SBC A, E", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::LOW, &DE}, //0x9B
      {"SBC A, H", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0x9C
      {"SBC A, L", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::LOW, &HL}, //0x9D
      {"SBC A, HL", 8, &CPU::SBC, IMode::HIGH, &AF, IMode::MEM, &HL}, //0x9E
      {"SBC A, A", 4, &CPU::SBC, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0x9F
      {"AND A, B", 4, &CPU::AND, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xA0
      {"AND A, C", 4, &CPU::AND, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xA1
      {"AND A, D", 4, &CPU::AND, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xA2
      {"AND A, E", 4, &CPU::AND, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xA3
      {"AND A, H", 4, &CPU::AND, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xA4
      {"AND A, L", 4, &CPU::AND, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xA5
      {"AND A, HL", 8, &CPU::AND, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xA6
      {"AND A, A", 4, &CPU::AND, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xA7
      {"XOR A, B", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xA8
      {"XOR A, C", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xA9
      {"XOR A, D", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xAA
      {"XOR A, E", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xAB
      {"XOR A, H", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xAC
      {"XOR A, L", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xAD
      {"XOR A, HL", 8, &CPU::XOR, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xAE
      {"XOR A, A", 4, &CPU::XOR, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xAF
      {"OR A, B", 4, &CPU::OR, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xB0
      {"OR A, C", 4, &CPU::OR, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xB1
      {"OR A, D", 4, &CPU::OR, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xB2
      {"OR A, E", 4, &CPU::OR, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xB3
      {"OR A, H", 4, &CPU::OR, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xB4
      {"OR A, L", 4, &CPU::OR, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xB5
      {"OR A, HL", 8, &CPU::OR, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xB6
      {"OR A, A", 4, &CPU::OR, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xB7
      {"CP A, B", 4, &CPU::CP, IMode::HIGH, &AF, IMode::HIGH, &BC}, //0xB8
      {"CP A, C", 4, &CPU::CP, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xB9
      {"CP A, D", 4, &CPU::CP, IMode::HIGH, &AF, IMode::HIGH, &DE}, //0xBA
      {"CP A, E", 4, &CPU::CP, IMode::HIGH, &AF, IMode::LOW, &DE}, //0xBB
      {"CP A, H", 4, &CPU::CP, IMode::HIGH, &AF, IMode::HIGH, &HL}, //0xBC
      {"CP A, L", 4, &CPU::CP, IMode::HIGH, &AF, IMode::LOW, &HL}, //0xBD
      {"CP A, HL", 8, &CPU::CP, IMode::HIGH, &AF, IMode::MEM, &HL}, //0xBE
      {"CP A, A", 4, &CPU::CP, IMode::HIGH, &AF, IMode::HIGH, &AF}, //0xBF
      {"RET NZ", 20, &CPU::RET, IMode::NZ, nullptr, IMode::NONE, nullptr}, //0xC0
      {"POP BC", 12, &CPU::POP, IMode::NONE, &BC, IMode::NONE, nullptr}, //0xC1
      {"JP NZ, a16", 16, &CPU::JP, IMode::NZ, nullptr, IMode::A16, nullptr}, //0xC2
      {"JP a16", 16, &CPU::JP, IMode::A16, nullptr, IMode::NONE, nullptr}, //0xC3
      {"CALL NZ, a16", 24, &CPU::CALL, IMode::NZ, nullptr, IMode::A16, nullptr}, //0xC4
      {"PUSH BC", 16, &CPU::PUSH, IMode::NONE, &BC, IMode::NONE, nullptr}, //0xC5
      {"ADD A, n8", 8, &CPU::ADD, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xC6
      {"RST $00", 16, &CPU::RST, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xC7
      {"RET Z", 20, &CPU::RET, IMode::Z, nullptr, IMode::NONE, nullptr}, //0xC8
      {"RET", 16, &CPU::RET, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xC9
      {"JP Z, a16", 16, &CPU::JP, IMode::Z, nullptr, IMode::A16, nullptr}, //0xCA
      {"PREFIX", 4, &CPU::PREFIX, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xCB
      {"CALL Z, a16", 24, &CPU::CALL, IMode::Z, nullptr, IMode::A16, nullptr}, //0xCC
      {"CALL a16", 24, &CPU::CALL, IMode::A16, nullptr, IMode::NONE, nullptr}, //0xCD
      {"ADC A, n8", 8, &CPU::ADC, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xCE
      {"RST $08", 16, &CPU::RST, IMode::R8, nullptr, IMode::NONE, nullptr}, //0xCF
      {"RET NC", 20, &CPU::RET, IMode::NC, nullptr, IMode::NONE, nullptr}, //0xD0
      {"POP DE", 12, &CPU::POP, IMode::NONE, &DE, IMode::NONE, nullptr}, //0xD1
      {"JP NC, a16", 16, &CPU::JP, IMode::NC, nullptr, IMode::A16, nullptr}, //0xD2
      {"ILLEGAL_D3", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xD3
      {"CALL NC, a16", 24, &CPU::CALL, IMode::NC, nullptr, IMode::A16, nullptr}, //0xD4
      {"PUSH DE", 16, &CPU::PUSH, IMode::NONE, &DE, IMode::NONE, nullptr}, //0xD5
      {"SUB A, n8", 8, &CPU::SUB, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xD6
      {"RST $10", 16, &CPU::RST, IMode::R10, nullptr, IMode::NONE, nullptr}, //0xD7
      {"RET C", 20, &CPU::RET, IMode::C, nullptr, IMode::NONE, nullptr}, //0xD8
      {"RETI", 16, &CPU::RETI, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xD9
      {"JP C, a16", 16, &CPU::JP, IMode::C, nullptr, IMode::A16, nullptr}, //0xDA
      {"ILLEGAL_DB", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xDB
      {"CALL C, a16", 24, &CPU::CALL, IMode::C, nullptr, IMode::A16, nullptr}, //0xDC
      {"ILLEGAL_DD", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xDD
      {"SBC A, n8", 8, &CPU::SBC, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xDE
      {"RST $18", 16, &CPU::RST, IMode::R18, nullptr, IMode::NONE, nullptr}, //0xDF
      {"LDH a8, A", 12, &CPU::LDH, IMode::A8, nullptr, IMode::HIGH, &AF}, //0xE0
      {"POP HL", 12, &CPU::POP, IMode::NONE, &HL, IMode::NONE, nullptr}, //0xE1
      {"LD C, A", 8, &CPU::LDS, IMode::LOW, &BC, IMode::HIGH, &AF}, //0xE2
      {"ILLEGAL_E3", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xE3
      {"ILLEGAL_E4", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xE4
      {"PUSH HL", 16, &CPU::PUSH, IMode::NONE, &HL, IMode::NONE, nullptr}, //0xE5
      {"AND A, n8", 8, &CPU::AND, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xE6
      {"RST $20", 16, &CPU::RST, IMode::R20, nullptr, IMode::NONE, nullptr}, //0xE7
      {"ADD SP, e8", 16, &CPU::ADD, IMode::ALL, &SP, IMode::E8, nullptr}, //0xE8
      {"JP HL", 4, &CPU::JP, IMode::ALL, &HL, IMode::NONE, nullptr}, //0xE9
      {"LD a16, A", 16, &CPU::LD, IMode::A16, nullptr, IMode::HIGH, &AF}, //0xEA
      {"ILLEGAL_EB", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xEB
      {"ILLEGAL_EC", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xEC
      {"ILLEGAL_ED", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xED
      {"XOR A, n8", 8, &CPU::XOR, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xEE
      {"RST $28", 16, &CPU::RST, IMode::R28, nullptr, IMode::NONE, nullptr}, //0xEF
      {"LDH A, a8", 12, &CPU::LDH, IMode::HIGH, &AF, IMode::A8, nullptr}, //0xF0
      {"POP AF", 12, &CPU::POP, IMode::NONE, &AF, IMode::NONE, nullptr}, //0xF1
      {"LD A, C", 8, &CPU::LDS, IMode::HIGH, &AF, IMode::LOW, &BC}, //0xF2
      {"DI", 4, &CPU::DI, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xF3
      {"ILLEGAL_F4", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xF4
      {"PUSH AF", 16, &CPU::PUSH, IMode::NONE, &AF, IMode::NONE, nullptr}, //0xF5
      {"OR A, n8", 8, &CPU::OR, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xF6
      {"RST $30", 16, &CPU::RST, IMode::R30, nullptr, IMode::NONE, nullptr}, //0xF7
      {"LD HL, SP + e8", 12, &CPU::LD, IMode::ALL, &HL, IMode::E8, &SP}, //0xF8
      {"LD SP, HL", 8, &CPU::LD, IMode::ALL, &SP, IMode::ALL, &HL}, //0xF9
      {"LD A, a16", 16, &CPU::LD, IMode::HIGH, &AF, IMode::A16, nullptr}, //0xFA
      {"EI", 4, &CPU::EI, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xFB
      {"ILLEGAL_FC", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xFC
      {"ILLEGAL_FD", 4, nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr}, //0xFD
      {"CP A, n8", 8, &CPU::CP, IMode::HIGH, &AF, IMode::N8, nullptr}, //0xFE
      {"RST $38", 16, &CPU::RST, IMode::R38, nullptr, IMode::NONE, nullptr} //0xFF
    };

    Instruction lookup_cb[256] =
    {
      {"RLC B", 8, &CPU::RLC, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x00
      {"RLC C", 8, &CPU::RLC, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x01
      {"RLC D", 8, &CPU::RLC, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x02
      {"RLC E", 8, &CPU::RLC, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x03
      {"RLC H", 8, &CPU::RLC, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x04
      {"RLC L", 8, &CPU::RLC, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x05
      {"RLC HL", 16, &CPU::RLC, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x06
      {"RLC A", 8, &CPU::RLC, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x07
      {"RRC B", 8, &CPU::RRC, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x08
      {"RRC C", 8, &CPU::RRC, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x09
      {"RRC D", 8, &CPU::RRC, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x0A
      {"RRC E", 8, &CPU::RRC, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x0B
      {"RRC H", 8, &CPU::RRC, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x0C
      {"RRC L", 8, &CPU::RRC, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x0D
      {"RRC HL", 16, &CPU::RRC, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x0E
      {"RRC A", 8, &CPU::RRC, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x0F
      {"RL B", 8, &CPU::RL, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x10
      {"RL C", 8, &CPU::RL, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x11
      {"RL D", 8, &CPU::RL, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x12
      {"RL E", 8, &CPU::RL, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x13
      {"RL H", 8, &CPU::RL, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x14
      {"RL L", 8, &CPU::RL, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x15
      {"RL HL", 16, &CPU::RL, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x16
      {"RL A", 8, &CPU::RL, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x17
      {"RR B", 8, &CPU::RR, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x18
      {"RR C", 8, &CPU::RR, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x19
      {"RR D", 8, &CPU::RR, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x1A
      {"RR E", 8, &CPU::RR, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x1B
      {"RR H", 8, &CPU::RR, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x1C
      {"RR L", 8, &CPU::RR, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x1D
      {"RR HL", 16, &CPU::RR, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x1E
      {"RR A", 8, &CPU::RR, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x1F
      {"SLA B", 8, &CPU::SLA, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x20
      {"SLA C", 8, &CPU::SLA, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x21
      {"SLA D", 8, &CPU::SLA, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x22
      {"SLA E", 8, &CPU::SLA, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x23
      {"SLA H", 8, &CPU::SLA, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x24
      {"SLA L", 8, &CPU::SLA, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x25
      {"SLA HL", 16, &CPU::SLA, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x26
      {"SLA A", 8, &CPU::SLA, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x27
      {"SRA B", 8, &CPU::SRA, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x28
      {"SRA C", 8, &CPU::SRA, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x29
      {"SRA D", 8, &CPU::SRA, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x2A
      {"SRA E", 8, &CPU::SRA, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x2B
      {"SRA H", 8, &CPU::SRA, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x2C
      {"SRA L", 8, &CPU::SRA, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x2D
      {"SRA HL", 16, &CPU::SRA, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x2E
      {"SRA A", 8, &CPU::SRA, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x2F
      {"SWAP B", 8, &CPU::SWAP, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x30
      {"SWAP C", 8, &CPU::SWAP, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x31
      {"SWAP D", 8, &CPU::SWAP, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x32
      {"SWAP E", 8, &CPU::SWAP, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x33
      {"SWAP H", 8, &CPU::SWAP, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x34
      {"SWAP L", 8, &CPU::SWAP, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x35
      {"SWAP HL", 16, &CPU::SWAP, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x36
      {"SWAP A", 8, &CPU::SWAP, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x37
      {"SRL B", 8, &CPU::SRL, IMode::HIGH, &BC, IMode::NONE, nullptr}, //0x38
      {"SRL C", 8, &CPU::SRL, IMode::LOW, &BC, IMode::NONE, nullptr}, //0x39
      {"SRL D", 8, &CPU::SRL, IMode::HIGH, &DE, IMode::NONE, nullptr}, //0x3A
      {"SRL E", 8, &CPU::SRL, IMode::LOW, &DE, IMode::NONE, nullptr}, //0x3B
      {"SRL H", 8, &CPU::SRL, IMode::HIGH, &HL, IMode::NONE, nullptr}, //0x3C
      {"SRL L", 8, &CPU::SRL, IMode::LOW, &HL, IMode::NONE, nullptr}, //0x3D
      {"SRL HL", 16, &CPU::SRL, IMode::MEM, &HL, IMode::NONE, nullptr}, //0x3E
      {"SRL A", 8, &CPU::SRL, IMode::HIGH, &AF, IMode::NONE, nullptr}, //0x3F
      {"BIT 0, B", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::HIGH, &BC}, //0x40
      {"BIT 0, C", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::LOW, &BC}, //0x41
      {"BIT 0, D", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::HIGH, &DE}, //0x42
      {"BIT 0, E", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::LOW, &DE}, //0x43
      {"BIT 0, H", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::HIGH, &HL}, //0x44
      {"BIT 0, L", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::LOW, &HL}, //0x45
      {"BIT 0, HL", 12, &CPU::IBIT, (IMode)0, nullptr, IMode::MEM, &HL}, //0x46
      {"BIT 0, A", 8, &CPU::IBIT, (IMode)0, nullptr, IMode::HIGH, &AF}, //0x47
      {"BIT 1, B", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::HIGH, &BC}, //0x48
      {"BIT 1, C", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::LOW, &BC}, //0x49
      {"BIT 1, D", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::HIGH, &DE}, //0x4A
      {"BIT 1, E", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::LOW, &DE}, //0x4B
      {"BIT 1, H", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::HIGH, &HL}, //0x4C
      {"BIT 1, L", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::LOW, &HL}, //0x4D
      {"BIT 1, HL", 12, &CPU::IBIT, (IMode)1, nullptr, IMode::MEM, &HL}, //0x4E
      {"BIT 1, A", 8, &CPU::IBIT, (IMode)1, nullptr, IMode::HIGH, &AF}, //0x4F
      {"BIT 2, B", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::HIGH, &BC}, //0x50
      {"BIT 2, C", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::LOW, &BC}, //0x51
      {"BIT 2, D", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::HIGH, &DE}, //0x52
      {"BIT 2, E", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::LOW, &DE}, //0x53
      {"BIT 2, H", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::HIGH, &HL}, //0x54
      {"BIT 2, L", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::LOW, &HL}, //0x55
      {"BIT 2, HL", 12, &CPU::IBIT, (IMode)2, nullptr, IMode::MEM, &HL}, //0x56
      {"BIT 2, A", 8, &CPU::IBIT, (IMode)2, nullptr, IMode::HIGH, &AF}, //0x57
      {"BIT 3, B", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::HIGH, &BC}, //0x58
      {"BIT 3, C", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::LOW, &BC}, //0x59
      {"BIT 3, D", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::HIGH, &DE}, //0x5A
      {"BIT 3, E", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::LOW, &DE}, //0x5B
      {"BIT 3, H", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::HIGH, &HL}, //0x5C
      {"BIT 3, L", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::LOW, &HL}, //0x5D
      {"BIT 3, HL", 12, &CPU::IBIT, (IMode)3, nullptr, IMode::MEM, &HL}, //0x5E
      {"BIT 3, A", 8, &CPU::IBIT, (IMode)3, nullptr, IMode::HIGH, &AF}, //0x5F
      {"BIT 4, B", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::HIGH, &BC}, //0x60
      {"BIT 4, C", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::LOW, &BC}, //0x61
      {"BIT 4, D", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::HIGH, &DE}, //0x62
      {"BIT 4, E", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::LOW, &DE}, //0x63
      {"BIT 4, H", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::HIGH, &HL}, //0x64
      {"BIT 4, L", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::LOW, &HL}, //0x65
      {"BIT 4, HL", 12, &CPU::IBIT, (IMode)4, nullptr, IMode::MEM, &HL}, //0x66
      {"BIT 4, A", 8, &CPU::IBIT, (IMode)4, nullptr, IMode::HIGH, &AF}, //0x67
      {"BIT 5, B", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::HIGH, &BC}, //0x68
      {"BIT 5, C", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::LOW, &BC}, //0x69
      {"BIT 5, D", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::HIGH, &DE}, //0x6A
      {"BIT 5, E", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::LOW, &DE}, //0x6B
      {"BIT 5, H", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::HIGH, &HL}, //0x6C
      {"BIT 5, L", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::LOW, &HL}, //0x6D
      {"BIT 5, HL", 12, &CPU::IBIT, (IMode)5, nullptr, IMode::MEM, &HL}, //0x6E
      {"BIT 5, A", 8, &CPU::IBIT, (IMode)5, nullptr, IMode::HIGH, &AF}, //0x6F
      {"BIT 6, B", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::HIGH, &BC}, //0x70
      {"BIT 6, C", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::LOW, &BC}, //0x71
      {"BIT 6, D", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::HIGH, &DE}, //0x72
      {"BIT 6, E", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::LOW, &DE}, //0x73
      {"BIT 6, H", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::HIGH, &HL}, //0x74
      {"BIT 6, L", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::LOW, &HL}, //0x75
      {"BIT 6, HL", 12, &CPU::IBIT, (IMode)6, nullptr, IMode::MEM, &HL}, //0x76
      {"BIT 6, A", 8, &CPU::IBIT, (IMode)6, nullptr, IMode::HIGH, &AF}, //0x77
      {"BIT 7, B", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::HIGH, &BC}, //0x78
      {"BIT 7, C", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::LOW, &BC}, //0x79
      {"BIT 7, D", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::HIGH, &DE}, //0x7A
      {"BIT 7, E", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::LOW, &DE}, //0x7B
      {"BIT 7, H", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::HIGH, &HL}, //0x7C
      {"BIT 7, L", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::LOW, &HL}, //0x7D
      {"BIT 7, HL", 12, &CPU::IBIT, (IMode)7, nullptr, IMode::MEM, &HL}, //0x7E
      {"BIT 7, A", 8, &CPU::IBIT, (IMode)7, nullptr, IMode::HIGH, &AF}, //0x7F
      {"RES 0, B", 8, &CPU::RES, (IMode)0, nullptr, IMode::HIGH, &BC}, //0x80
      {"RES 0, C", 8, &CPU::RES, (IMode)0, nullptr, IMode::LOW, &BC}, //0x81
      {"RES 0, D", 8, &CPU::RES, (IMode)0, nullptr, IMode::HIGH, &DE}, //0x82
      {"RES 0, E", 8, &CPU::RES, (IMode)0, nullptr, IMode::LOW, &DE}, //0x83
      {"RES 0, H", 8, &CPU::RES, (IMode)0, nullptr, IMode::HIGH, &HL}, //0x84
      {"RES 0, L", 8, &CPU::RES, (IMode)0, nullptr, IMode::LOW, &HL}, //0x85
      {"RES 0, HL", 16, &CPU::RES, (IMode)0, nullptr, IMode::MEM, &HL}, //0x86
      {"RES 0, A", 8, &CPU::RES, (IMode)0, nullptr, IMode::HIGH, &AF}, //0x87
      {"RES 1, B", 8, &CPU::RES, (IMode)1, nullptr, IMode::HIGH, &BC}, //0x88
      {"RES 1, C", 8, &CPU::RES, (IMode)1, nullptr, IMode::LOW, &BC}, //0x89
      {"RES 1, D", 8, &CPU::RES, (IMode)1, nullptr, IMode::HIGH, &DE}, //0x8A
      {"RES 1, E", 8, &CPU::RES, (IMode)1, nullptr, IMode::LOW, &DE}, //0x8B
      {"RES 1, H", 8, &CPU::RES, (IMode)1, nullptr, IMode::HIGH, &HL}, //0x8C
      {"RES 1, L", 8, &CPU::RES, (IMode)1, nullptr, IMode::LOW, &HL}, //0x8D
      {"RES 1, HL", 16, &CPU::RES, (IMode)1, nullptr, IMode::MEM, &HL}, //0x8E
      {"RES 1, A", 8, &CPU::RES, (IMode)1, nullptr, IMode::HIGH, &AF}, //0x8F
      {"RES 2, B", 8, &CPU::RES, (IMode)2, nullptr, IMode::HIGH, &BC}, //0x90
      {"RES 2, C", 8, &CPU::RES, (IMode)2, nullptr, IMode::LOW, &BC}, //0x91
      {"RES 2, D", 8, &CPU::RES, (IMode)2, nullptr, IMode::HIGH, &DE}, //0x92
      {"RES 2, E", 8, &CPU::RES, (IMode)2, nullptr, IMode::LOW, &DE}, //0x93
      {"RES 2, H", 8, &CPU::RES, (IMode)2, nullptr, IMode::HIGH, &HL}, //0x94
      {"RES 2, L", 8, &CPU::RES, (IMode)2, nullptr, IMode::LOW, &HL}, //0x95
      {"RES 2, HL", 16, &CPU::RES, (IMode)2, nullptr, IMode::MEM, &HL}, //0x96
      {"RES 2, A", 8, &CPU::RES, (IMode)2, nullptr, IMode::HIGH, &AF}, //0x97
      {"RES 3, B", 8, &CPU::RES, (IMode)3, nullptr, IMode::HIGH, &BC}, //0x98
      {"RES 3, C", 8, &CPU::RES, (IMode)3, nullptr, IMode::LOW, &BC}, //0x99
      {"RES 3, D", 8, &CPU::RES, (IMode)3, nullptr, IMode::HIGH, &DE}, //0x9A
      {"RES 3, E", 8, &CPU::RES, (IMode)3, nullptr, IMode::LOW, &DE}, //0x9B
      {"RES 3, H", 8, &CPU::RES, (IMode)3, nullptr, IMode::HIGH, &HL}, //0x9C
      {"RES 3, L", 8, &CPU::RES, (IMode)3, nullptr, IMode::LOW, &HL}, //0x9D
      {"RES 3, HL", 16, &CPU::RES, (IMode)3, nullptr, IMode::MEM, &HL}, //0x9E
      {"RES 3, A", 8, &CPU::RES, (IMode)3, nullptr, IMode::HIGH, &AF}, //0x9F
      {"RES 4, B", 8, &CPU::RES, (IMode)4, nullptr, IMode::HIGH, &BC}, //0xA0
      {"RES 4, C", 8, &CPU::RES, (IMode)4, nullptr, IMode::LOW, &BC}, //0xA1
      {"RES 4, D", 8, &CPU::RES, (IMode)4, nullptr, IMode::HIGH, &DE}, //0xA2
      {"RES 4, E", 8, &CPU::RES, (IMode)4, nullptr, IMode::LOW, &DE}, //0xA3
      {"RES 4, H", 8, &CPU::RES, (IMode)4, nullptr, IMode::HIGH, &HL}, //0xA4
      {"RES 4, L", 8, &CPU::RES, (IMode)4, nullptr, IMode::LOW, &HL}, //0xA5
      {"RES 4, HL", 16, &CPU::RES, (IMode)4, nullptr, IMode::MEM, &HL}, //0xA6
      {"RES 4, A", 8, &CPU::RES, (IMode)4, nullptr, IMode::HIGH, &AF}, //0xA7
      {"RES 5, B", 8, &CPU::RES, (IMode)5, nullptr, IMode::HIGH, &BC}, //0xA8
      {"RES 5, C", 8, &CPU::RES, (IMode)5, nullptr, IMode::LOW, &BC}, //0xA9
      {"RES 5, D", 8, &CPU::RES, (IMode)5, nullptr, IMode::HIGH, &DE}, //0xAA
      {"RES 5, E", 8, &CPU::RES, (IMode)5, nullptr, IMode::LOW, &DE}, //0xAB
      {"RES 5, H", 8, &CPU::RES, (IMode)5, nullptr, IMode::HIGH, &HL}, //0xAC
      {"RES 5, L", 8, &CPU::RES, (IMode)5, nullptr, IMode::LOW, &HL}, //0xAD
      {"RES 5, HL", 16, &CPU::RES, (IMode)5, nullptr, IMode::MEM, &HL}, //0xAE
      {"RES 5, A", 8, &CPU::RES, (IMode)5, nullptr, IMode::HIGH, &AF}, //0xAF
      {"RES 6, B", 8, &CPU::RES, (IMode)6, nullptr, IMode::HIGH, &BC}, //0xB0
      {"RES 6, C", 8, &CPU::RES, (IMode)6, nullptr, IMode::LOW, &BC}, //0xB1
      {"RES 6, D", 8, &CPU::RES, (IMode)6, nullptr, IMode::HIGH, &DE}, //0xB2
      {"RES 6, E", 8, &CPU::RES, (IMode)6, nullptr, IMode::LOW, &DE}, //0xB3
      {"RES 6, H", 8, &CPU::RES, (IMode)6, nullptr, IMode::HIGH, &HL}, //0xB4
      {"RES 6, L", 8, &CPU::RES, (IMode)6, nullptr, IMode::LOW, &HL}, //0xB5
      {"RES 6, HL", 16, &CPU::RES, (IMode)6, nullptr, IMode::MEM, &HL}, //0xB6
      {"RES 6, A", 8, &CPU::RES, (IMode)6, nullptr, IMode::HIGH, &AF}, //0xB7
      {"RES 7, B", 8, &CPU::RES, (IMode)7, nullptr, IMode::HIGH, &BC}, //0xB8
      {"RES 7, C", 8, &CPU::RES, (IMode)7, nullptr, IMode::LOW, &BC}, //0xB9
      {"RES 7, D", 8, &CPU::RES, (IMode)7, nullptr, IMode::HIGH, &DE}, //0xBA
      {"RES 7, E", 8, &CPU::RES, (IMode)7, nullptr, IMode::LOW, &DE}, //0xBB
      {"RES 7, H", 8, &CPU::RES, (IMode)7, nullptr, IMode::HIGH, &HL}, //0xBC
      {"RES 7, L", 8, &CPU::RES, (IMode)7, nullptr, IMode::LOW, &HL}, //0xBD
      {"RES 7, HL", 16, &CPU::RES, (IMode)7, nullptr, IMode::MEM, &HL}, //0xBE
      {"RES 7, A", 8, &CPU::RES, (IMode)7, nullptr, IMode::HIGH, &AF}, //0xBF
      {"SET 0, B", 8, &CPU::SET, (IMode)0, nullptr, IMode::HIGH, &BC}, //0xC0
      {"SET 0, C", 8, &CPU::SET, (IMode)0, nullptr, IMode::LOW, &BC}, //0xC1
      {"SET 0, D", 8, &CPU::SET, (IMode)0, nullptr, IMode::HIGH, &DE}, //0xC2
      {"SET 0, E", 8, &CPU::SET, (IMode)0, nullptr, IMode::LOW, &DE}, //0xC3
      {"SET 0, H", 8, &CPU::SET, (IMode)0, nullptr, IMode::HIGH, &HL}, //0xC4
      {"SET 0, L", 8, &CPU::SET, (IMode)0, nullptr, IMode::LOW, &HL}, //0xC5
      {"SET 0, HL", 16, &CPU::SET, (IMode)0, nullptr, IMode::MEM, &HL}, //0xC6
      {"SET 0, A", 8, &CPU::SET, (IMode)0, nullptr, IMode::HIGH, &AF}, //0xC7
      {"SET 1, B", 8, &CPU::SET, (IMode)1, nullptr, IMode::HIGH, &BC}, //0xC8
      {"SET 1, C", 8, &CPU::SET, (IMode)1, nullptr, IMode::LOW, &BC}, //0xC9
      {"SET 1, D", 8, &CPU::SET, (IMode)1, nullptr, IMode::HIGH, &DE}, //0xCA
      {"SET 1, E", 8, &CPU::SET, (IMode)1, nullptr, IMode::LOW, &DE}, //0xCB
      {"SET 1, H", 8, &CPU::SET, (IMode)1, nullptr, IMode::HIGH, &HL}, //0xCC
      {"SET 1, L", 8, &CPU::SET, (IMode)1, nullptr, IMode::LOW, &HL}, //0xCD
      {"SET 1, HL", 16, &CPU::SET, (IMode)1, nullptr, IMode::MEM, &HL}, //0xCE
      {"SET 1, A", 8, &CPU::SET, (IMode)1, nullptr, IMode::HIGH, &AF}, //0xCF
      {"SET 2, B", 8, &CPU::SET, (IMode)2, nullptr, IMode::HIGH, &BC}, //0xD0
      {"SET 2, C", 8, &CPU::SET, (IMode)2, nullptr, IMode::LOW, &BC}, //0xD1
      {"SET 2, D", 8, &CPU::SET, (IMode)2, nullptr, IMode::HIGH, &DE}, //0xD2
      {"SET 2, E", 8, &CPU::SET, (IMode)2, nullptr, IMode::LOW, &DE}, //0xD3
      {"SET 2, H", 8, &CPU::SET, (IMode)2, nullptr, IMode::HIGH, &HL}, //0xD4
      {"SET 2, L", 8, &CPU::SET, (IMode)2, nullptr, IMode::LOW, &HL}, //0xD5
      {"SET 2, HL", 16, &CPU::SET, (IMode)2, nullptr, IMode::MEM, &HL}, //0xD6
      {"SET 2, A", 8, &CPU::SET, (IMode)2, nullptr, IMode::HIGH, &AF}, //0xD7
      {"SET 3, B", 8, &CPU::SET, (IMode)3, nullptr, IMode::HIGH, &BC}, //0xD8
      {"SET 3, C", 8, &CPU::SET, (IMode)3, nullptr, IMode::LOW, &BC}, //0xD9
      {"SET 3, D", 8, &CPU::SET, (IMode)3, nullptr, IMode::HIGH, &DE}, //0xDA
      {"SET 3, E", 8, &CPU::SET, (IMode)3, nullptr, IMode::LOW, &DE}, //0xDB
      {"SET 3, H", 8, &CPU::SET, (IMode)3, nullptr, IMode::HIGH, &HL}, //0xDC
      {"SET 3, L", 8, &CPU::SET, (IMode)3, nullptr, IMode::LOW, &HL}, //0xDD
      {"SET 3, HL", 16, &CPU::SET, (IMode)3, nullptr, IMode::MEM, &HL}, //0xDE
      {"SET 3, A", 8, &CPU::SET, (IMode)3, nullptr, IMode::HIGH, &AF}, //0xDF
      {"SET 4, B", 8, &CPU::SET, (IMode)4, nullptr, IMode::HIGH, &BC}, //0xE0
      {"SET 4, C", 8, &CPU::SET, (IMode)4, nullptr, IMode::LOW, &BC}, //0xE1
      {"SET 4, D", 8, &CPU::SET, (IMode)4, nullptr, IMode::HIGH, &DE}, //0xE2
      {"SET 4, E", 8, &CPU::SET, (IMode)4, nullptr, IMode::LOW, &DE}, //0xE3
      {"SET 4, H", 8, &CPU::SET, (IMode)4, nullptr, IMode::HIGH, &HL}, //0xE4
      {"SET 4, L", 8, &CPU::SET, (IMode)4, nullptr, IMode::LOW, &HL}, //0xE5
      {"SET 4, HL", 16, &CPU::SET, (IMode)4, nullptr, IMode::MEM, &HL}, //0xE6
      {"SET 4, A", 8, &CPU::SET, (IMode)4, nullptr, IMode::HIGH, &AF}, //0xE7
      {"SET 5, B", 8, &CPU::SET, (IMode)5, nullptr, IMode::HIGH, &BC}, //0xE8
      {"SET 5, C", 8, &CPU::SET, (IMode)5, nullptr, IMode::LOW, &BC}, //0xE9
      {"SET 5, D", 8, &CPU::SET, (IMode)5, nullptr, IMode::HIGH, &DE}, //0xEA
      {"SET 5, E", 8, &CPU::SET, (IMode)5, nullptr, IMode::LOW, &DE}, //0xEB
      {"SET 5, H", 8, &CPU::SET, (IMode)5, nullptr, IMode::HIGH, &HL}, //0xEC
      {"SET 5, L", 8, &CPU::SET, (IMode)5, nullptr, IMode::LOW, &HL}, //0xED
      {"SET 5, HL", 16, &CPU::SET, (IMode)5, nullptr, IMode::MEM, &HL}, //0xEE
      {"SET 5, A", 8, &CPU::SET, (IMode)5, nullptr, IMode::HIGH, &AF}, //0xEF
      {"SET 6, B", 8, &CPU::SET, (IMode)6, nullptr, IMode::HIGH, &BC}, //0xF0
      {"SET 6, C", 8, &CPU::SET, (IMode)6, nullptr, IMode::LOW, &BC}, //0xF1
      {"SET 6, D", 8, &CPU::SET, (IMode)6, nullptr, IMode::HIGH, &DE}, //0xF2
      {"SET 6, E", 8, &CPU::SET, (IMode)6, nullptr, IMode::LOW, &DE}, //0xF3
      {"SET 6, H", 8, &CPU::SET, (IMode)6, nullptr, IMode::HIGH, &HL}, //0xF4
      {"SET 6, L", 8, &CPU::SET, (IMode)6, nullptr, IMode::LOW, &HL}, //0xF5
      {"SET 6, HL", 16, &CPU::SET, (IMode)6, nullptr, IMode::MEM, &HL}, //0xF6
      {"SET 6, A", 8, &CPU::SET, (IMode)6, nullptr, IMode::HIGH, &AF}, //0xF7
      {"SET 7, B", 8, &CPU::SET, (IMode)7, nullptr, IMode::HIGH, &BC}, //0xF8
      {"SET 7, C", 8, &CPU::SET, (IMode)7, nullptr, IMode::LOW, &BC}, //0xF9
      {"SET 7, D", 8, &CPU::SET, (IMode)7, nullptr, IMode::HIGH, &DE}, //0xFA
      {"SET 7, E", 8, &CPU::SET, (IMode)7, nullptr, IMode::LOW, &DE}, //0xFB
      {"SET 7, H", 8, &CPU::SET, (IMode)7, nullptr, IMode::HIGH, &HL}, //0xFC
      {"SET 7, L", 8, &CPU::SET, (IMode)7, nullptr, IMode::LOW, &HL}, //0xFD
      {"SET 7, HL", 16, &CPU::SET, (IMode)7, nullptr, IMode::MEM, &HL}, //0xFE
      {"SET 7, A", 8, &CPU::SET, (IMode)7, nullptr, IMode::HIGH, &AF}, //0xFF
    };
    
  };


};
