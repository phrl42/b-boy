#pragma once
#include "renderer/Context.hpp"

namespace Banana
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(void* window);
		virtual ~OpenGLContext() override = default;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void SwapBuffer() override;

	private:
		void* window;
	};
};
