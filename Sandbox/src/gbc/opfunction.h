#pragma once
#include "Sandbox.h"

namespace GBC
{
  void LD(uint16_t *dest_register, uint16_t src_value);

  void INC(uint16_t *dest_register, bool higher_half);

};
