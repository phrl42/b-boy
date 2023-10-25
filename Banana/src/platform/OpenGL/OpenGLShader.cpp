#include "incs.h"
#include "platform/OpenGL/OpenGLShader.h"

#include "glad.h"
#include "GLFW/glfw3.h"

#include "gtc/type_ptr.hpp"

namespace Banana
{
  OpenGLShader::OpenGLShader(const std::string& file_path)
  : file_path(file_path)
  {
    std::basic_ifstream<char> ifs(file_path);
    
    // save file content into string
    std::string content((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

    if(content.empty())
    {
      LOG("Could not find shader: " + file_path);
      return;
    }
    
    int vpos = content.find("#type vertex");
    int fpos = content.find("#type fragment");

    for(int i = vpos + 13; i < fpos; i++)
    {
      vertex_src += content[i];
    }

    for(int i = fpos + 15; i < content.size(); i++)
    {
      fragment_src += content[i];
    }
  
  }

  void OpenGLShader::Bind()
  {
    glUseProgram(id);
  }

  void OpenGLShader::Unbind()
  {
    glUseProgram(0);
  }

  void OpenGLShader::Compile()
  {
    unsigned int shaderID = 0;
    unsigned int vertexID = 4242;
    unsigned int fragmentID = 0;

    int success = 0;

    char log[200];
    memset(log, '\0', sizeof(char) * 200);
    
    vertexID = glCreateShader(GL_VERTEX_SHADER);

    // the hell is this casting
    const char* vertex = vertex_src.c_str();
    glShaderSource(vertexID, 1, &vertex, 0);

    glCompileShader(vertexID);

    // error handling
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      glGetShaderInfoLog(vertexID, 200, NULL, log);
      LOG("Compiling vertex shader failed: " + std::string(log));
      return;
    }

    // the same applies for the fragment shader once again
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    const char* fragmentshader = fragment_src.c_str();
    glShaderSource(fragmentID, 1, &fragmentshader, NULL);

    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
      glGetShaderInfoLog(fragmentID, GL_COMPILE_STATUS, NULL, log);
      LOG("Compiling fragment shader failed: " + std::string(log));
      return;
    }
    // link shader

    shaderID = glCreateProgram();

    glAttachShader(shaderID, vertexID);
    glAttachShader(shaderID, fragmentID);

    glLinkProgram(shaderID);

    // delete unused shaders now
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    LOG("Compiled shader: " + file_path);
    this->id = shaderID;
  }

  void OpenGLShader::UploadMat4f(const char* varName, glm::mat4 mat4) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat4));
  }

  void OpenGLShader::UploadMat3f(const char* varName, glm::mat3 mat3)
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniformMatrix3fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat3));
  }

  void OpenGLShader::UploadVec4f(const char* varName, glm::vec4 vec4) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform4f(varLocation, vec4.x, vec4.y, vec4.z, vec4.w);
  }
  void OpenGLShader::UploadVec3f(const char* varName, glm::vec3 vec3) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform3f(varLocation, vec3.x, vec3.y, vec3.z);
  }

  void OpenGLShader::UploadVec2f(const char* varName, glm::vec2 vec2) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform2f(varLocation, vec2.x, vec2.y);
  }

  void OpenGLShader::UploadFloat(const char* varName, float value) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform1f(varLocation, value);
  }

  void OpenGLShader::UploadInt(const char* varName, int value) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform1i(varLocation, value);
  }
  
  void OpenGLShader::UploadIntArray(const char* varName, int arrayLength, int array[]) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform1iv(varLocation, arrayLength, array);
  }

  void OpenGLShader::UploadTexture(const char* varName, int slot) 
  {
    int varLocation = glGetUniformLocation(id, varName);
    Bind();
    glUniform1i(varLocation, slot);
  }

};