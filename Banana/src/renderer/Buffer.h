#pragma once
#include "utility.h"

namespace Banana
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Bool: return 1;
		}

		LOG("Unknown ShaderDataType in size. Did you miss something?");
		return 0;
	}

	struct BufferElement
	{
	public:
		std::string Name;

		uint32_t Offset;
		uint32_t Size;
		uint32_t Count;
		bool Normalized;

		ShaderDataType Type;

		BufferElement()
		{
		}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		int GetTypeCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;
			case ShaderDataType::Mat3: return 3;
			case ShaderDataType::Mat4: return 4;
			case ShaderDataType::Bool: return 1;
			}

			LOG("Could not determine type count");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout()
		{
		}

		BufferLayout(const std::initializer_list<BufferElement>& in_elements)
			: elements(in_elements)
		{
			CalculateOffsetAndStride();
		}

		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
		inline const std::vector<BufferElement>& GetElements() const { return elements; }

		inline uint32_t GetStride() const { return stride; }

	private:
		std::vector<BufferElement> elements;
		uint32_t stride = 0;

		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			stride = 0;
			for (auto& element : elements)
			{
				element.Offset = offset;
				offset += element.Size;
				stride += element.Size;
			}
		}
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() = 0;

		static Shr<VertexBuffer> Create(const BufferLayout& layout, uint32_t size);

		virtual uint32_t GetCount() = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetCount() = 0;

		static Shr<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};
}
