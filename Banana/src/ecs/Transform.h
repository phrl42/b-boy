#pragma once

#include "Projection.h"

namespace Banana
{
  class Transform
  {
  public:
    ~Transform() = default;

    Transform() = default;
    Transform(const glm::vec3 &pos, const glm::vec3& size, const glm::vec4 &color, float rotation, Projection proj);

  public:
    glm::vec3 pos;
    glm::vec3 size;
    glm::vec4 color;
    float rotation = 0;

    Projection proj = Projection::NONE;
  };
};