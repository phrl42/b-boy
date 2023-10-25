#include "incs.h"
#include "generic/Camera.h"

#include "fwd.hpp"
#include "gtc/matrix_transform.hpp"

#include "Application.hpp"

namespace Banana
{
  Camera::Camera()
  {
    CalcMatrix();
  }

  void Camera::SetWindowDimension(unsigned int width, unsigned int height)
  {
    this->width = width; 
    this->height = height;
    
    perspective_projection = glm::perspective(glm::radians(90.0f), 
        (float)width/(float)height, 0.1f, 1000.0f);
    
    // I will never understand this
    orthographic_projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 1000.0f);
    
    view_matrix = glm::mat4(1.0f);

    CalcMatrix();
  }

  Camera::~Camera()
  {

  }

  void Camera::CalcMatrix()
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
                          glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));

    view_matrix = glm::inverse(transform);

    perspective_view_projection = perspective_projection * view_matrix;
    orthographic_view_projection = orthographic_projection * view_matrix;
  }

};