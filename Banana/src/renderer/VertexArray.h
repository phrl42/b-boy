#pragma once

#include "renderer/Buffer.h"

namespace Banana
{
  class VertexArray
  {
  public:
  
    virtual ~VertexArray() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void AddVertexBuffer(const Shr<VertexBuffer>& vertex_buffer) = 0;
    virtual void SetIndexBuffer(const Shr<IndexBuffer>& index_buffer) = 0;

		virtual Shr<VertexBuffer>& GetVertexBuffer() = 0;
		virtual Shr<IndexBuffer>& GetElementBuffer() = 0;

    static Shr<VertexArray> Create();
  };
};