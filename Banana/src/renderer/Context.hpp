#pragma once
#include "utility.h"

namespace Banana
{
	class Context
	{
	public:
		Context() = default;
		virtual ~Context() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void SwapBuffer() = 0;

		static Context* CreateContext(void* window);
	};
};
