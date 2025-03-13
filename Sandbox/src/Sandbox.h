#pragma once

#include "Banana.h"

namespace SANDBOX
{
  class Sandbox : public Banana::Application
  {
  public:
    Sandbox();
    ~Sandbox() override;
  };
};