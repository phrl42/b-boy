#pragma once

struct Stats
{
  static inline uint16_t PC = 0;
  static inline uint16_t SP = 0;
  
  static inline uint16_t AF = 0;
  static inline uint16_t BC = 0;
  static inline uint16_t DE = 0;

  static inline uint16_t HL = 0;

  static inline uint8_t opval = 0;
private:
  inline Stats() {}
  inline ~Stats() {}
};
