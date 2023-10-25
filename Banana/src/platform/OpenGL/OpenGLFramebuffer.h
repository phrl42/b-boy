#pragma once
#include "renderer/Framebuffer.h"

namespace Banana
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		virtual ~OpenGLFramebuffer() override;

		OpenGLFramebuffer(const FramebufferProperties& fb);

		virtual void Shutdown() override;
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentID() override;

		virtual uint32_t GetID() override { return id; };

		virtual void SetWindowDimension(uint32_t width, uint32_t height) override;


		void Invalidate();

	private:
		uint32_t id;
		uint32_t color_id;
		uint32_t depth_id;
		FramebufferProperties specs;
	};
};
