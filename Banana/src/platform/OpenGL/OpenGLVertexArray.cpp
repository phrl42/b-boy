#include "incs.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

#include "glad.h"
#include "GLFW/glfw3.h"
namespace Banana
{
  static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
  {
    switch(type)
    {
      case ShaderDataType::Float: return GL_FLOAT;
      case ShaderDataType::Float2: return GL_FLOAT;
      case ShaderDataType::Float3: return GL_FLOAT;
      case ShaderDataType::Float4: return GL_FLOAT;
      case ShaderDataType::Int: return GL_INT;
      case ShaderDataType::Int2: return GL_INT;
      case ShaderDataType::Int3: return GL_INT;
      case ShaderDataType::Int4: return GL_INT;
      case ShaderDataType::Mat3: return GL_FLOAT;
      case ShaderDataType::Mat4: return GL_FLOAT;
      case ShaderDataType::Bool: return GL_BOOL;
    }

    LOG("Unknown ShaderDataType. Did you miss something?");
    return 0;
  }

  OpenGLVertexArray::OpenGLVertexArray()
  {
    glCreateVertexArrays(1, &id);
  }
  
  void OpenGLVertexArray::AddVertexBuffer(const Shr<VertexBuffer> &vertex_buffer)
  {
    glBindVertexArray(id);
    vertex_buffer->Bind();

    if(!vertex_buffer->GetLayout().GetElements().size())
    {
      LOG("There are no vertex buffer elements");
    }

    uint32_t index = 0;
    const auto& layout = vertex_buffer->GetLayout();
    for(const auto& element : layout)
    {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index,
        element.GetTypeCount(), 
        ShaderDataTypeToOpenGLType(element.Type),
        element.Normalized ? GL_TRUE : GL_FALSE, 
        layout.GetStride(), 
        (const void*)element.Offset);

      index += 1;
    }
    this->vertex_buffer = vertex_buffer;
  }

  void OpenGLVertexArray::SetIndexBuffer(const Shr<IndexBuffer> &index_buffer)
  {
    glBindVertexArray(id);
    index_buffer->Bind();

    this->index_buffer = index_buffer;
  }

  void OpenGLVertexArray::Bind()
  {
    if(vertex_buffer)
      vertex_buffer->Bind();

    if(index_buffer)
      index_buffer->Bind();

    glBindVertexArray(id);
  }

  void OpenGLVertexArray::Unbind()
  {
    glBindVertexArray(0);

    if(vertex_buffer)
      vertex_buffer->Unbind();

    if(index_buffer)
      index_buffer->Unbind();
  }

};