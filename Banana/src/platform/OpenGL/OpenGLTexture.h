#pragma once
#include "renderer/Texture.h"

namespace Banana
{
  class OpenGLTexture2D : public Texture2D
  {
  public:
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(const TextureSpecification& spec);
    virtual ~OpenGLTexture2D() override;

    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

    virtual void UpdateTexture(const TextureSpecification &spec) override;
    
    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;
    virtual uint32_t GetRendererID() const override { return id; }
    virtual bool operator==(const Texture& other) const override { return this->id == other.GetRendererID(); }

  private:
    std::string path;

    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t id = 0;
  };
};
