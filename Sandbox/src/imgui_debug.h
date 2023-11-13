#pragma once
#include "gbc/gbc.h"


struct Stats
{
  static inline GBC::Spec *spec;
  static inline uint32_t play_id;
  static inline uint32_t stop_id;
private:
  inline Stats() {}
  inline ~Stats() {}
};
