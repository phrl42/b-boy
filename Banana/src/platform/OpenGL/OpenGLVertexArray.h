#pragma once

#include "renderer/VertexArray.h"

namespace Banana
{
  class OpenGLVertexArray : public VertexArray 
  {
  public:
    virtual ~OpenGLVertexArray() override = default;
    OpenGLVertexArray();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void AddVertexBuffer(const Shr<VertexBuffer>& vertex_buffer) override;
    virtual void SetIndexBuffer(const Shr<IndexBuffer>& index_buffer) override;

		virtual Shr<VertexBuffer>& GetVertexBuffer() override { return vertex_buffer; }
		virtual Shr<IndexBuffer>& GetElementBuffer() override { return index_buffer; }

  private:
    Shr<VertexBuffer> vertex_buffer;
    Shr<IndexBuffer> index_buffer;

    unsigned int id;
  };
};