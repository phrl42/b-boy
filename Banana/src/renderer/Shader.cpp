#include "incs.h"
#include "platform/OpenGL/OpenGLShader.h"

#include "renderer/Renderer.h"

namespace Banana
{
  Shr<Shader> Shader::Create(const std::string& file_path)
  {
    switch(Renderer::GetAPI())
    {
      case RendererAPI::API::OpenGL: return MakeShr<OpenGLShader>(file_path);
    }

    LOG("Could not identify api in shader class. the hell are you doing??!");
    return nullptr;
  }
};