#include "renderer/Buffer.h"

namespace Banana
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const BufferLayout& layout, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() override { return layout; }

		virtual uint32_t GetCount() override { return size; }

	public:
		BufferLayout layout;
		uint32_t size;
		uint32_t id;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetCount() override { return count; }

	private:
		uint32_t id;
		uint32_t count;
	};
};
