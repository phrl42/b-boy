#pragma once


namespace Banana
{
  struct FramebufferProperties
  {
    uint32_t width;
    uint32_t height;
  };

  class Framebuffer
  {
  public:
    virtual ~Framebuffer() = default;

    virtual void Shutdown() = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual uint32_t GetID() = 0;
    
    virtual uint32_t GetColorAttachmentID() = 0;

    virtual void SetWindowDimension(uint32_t width, uint32_t height) = 0;

    static Shr<Framebuffer> Create(const FramebufferProperties& fb);
  };
};