#pragma once
#include "ecs/Transform.h"

namespace Banana
{
  class Component
  {
  public:
    virtual ~Component() = default;

    virtual void OnUpdate(float dt, const Transform &transform) = 0;

    const std::string& GetName() const { return name; }
  protected:
    std::string name;
  };

};