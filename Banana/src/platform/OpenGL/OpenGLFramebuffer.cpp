#include "incs.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"

#include "glad.h"
#include "GLFW/glfw3.h"

namespace Banana
{
  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProperties& fb)
  : specs(fb)
  {
    Invalidate();
  }

  void OpenGLFramebuffer::SetWindowDimension(uint32_t width, uint32_t height)
  {
    this->specs.width = width;
    this->specs.height = height;
    
    Shutdown();
    Invalidate();
  }

  void OpenGLFramebuffer::Invalidate()
  {
    glCreateFramebuffers(1, &id);
    Bind();

    // create color attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &color_id);
    glBindTexture(GL_TEXTURE_2D, color_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specs.width, specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_id, 0);

    // create depth attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &depth_id);
    glBindTexture(GL_TEXTURE_2D, depth_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specs.width, specs.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_id, 0);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      LOG("Could not create framebuffer");
    }

    Unbind();
  }

  uint32_t OpenGLFramebuffer::GetColorAttachmentID()
  {
    return color_id;
  }

  OpenGLFramebuffer::~OpenGLFramebuffer()
  {
    Shutdown();
  }

  void OpenGLFramebuffer::Shutdown()
  {
    glDeleteFramebuffers(1, &id);
  }

  void OpenGLFramebuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
  }

  void OpenGLFramebuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
};