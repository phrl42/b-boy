#pragma once
#include "glm.hpp"

namespace Banana
{
  class Camera 
  {
  public:
    Camera();
    ~Camera();

    inline void SetPosition(const glm::vec3& pos) { this->position = pos; CalcMatrix(); }
    inline void SetRotation(float rotation) { this->rotation = rotation; CalcMatrix(); }

    inline const glm::vec3& GetPosition() const { return this->position; }
    inline float GetRotation() const { return this->rotation; }

    inline const glm::mat4& GetViewMatrix() const { return this->view_matrix; }

    inline const glm::mat4& GetPerspectiveViewProjection() const { return this->perspective_view_projection; }
    inline const glm::mat4& GetOrthographicViewProjection() const { return this->orthographic_view_projection; }

    void SetWindowDimension(unsigned int width, unsigned int height);

  private:
    void CalcMatrix();
  private:
    glm::mat4 perspective_projection;
    glm::mat4 orthographic_projection;

    glm::mat4 view_matrix;

    glm::mat4 perspective_view_projection;
    glm::mat4 orthographic_view_projection;

    glm::vec3 position;
    float rotation = 0.0f;

    unsigned int width = 1;
    unsigned int height = 1;
  };
};