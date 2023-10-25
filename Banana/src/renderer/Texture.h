#pragma once

namespace Banana
{

  enum class ImageFormat
  {
    None = 0,
    R8,
    RGB8,
    RGBA8,
    RGBA32F
  };

  struct TextureSpecification
  {
    uint32_t width = 1;
    uint32_t height = 1;

    ImageFormat format = ImageFormat::RGB8;
    bool GenerateMips = true;

    void* data;
    uint32_t size = 0;
  };

  class Texture
  {
    public:
      virtual ~Texture() = default;

      virtual uint32_t GetWidth() const = 0;
      virtual uint32_t GetHeight() const = 0;

      virtual void Bind(uint32_t slot = 0) const = 0;
      virtual void Unbind() const = 0;

      virtual uint32_t GetRendererID() const = 0;

      virtual bool operator==(const Texture& other) const = 0;
  };

  class Texture2D : public Texture
  {
    public:
      static Shr<Texture2D> Create(const std::string& file_path);
      static Shr<Texture2D> Create(const TextureSpecification& spec);
  };
};
