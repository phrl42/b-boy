#pragma once
#include "event/Input.hpp"

namespace Banana
{
	class GLFWInput : public Input
	{
	public:
		GLFWInput() = default;
		~GLFWInput() override = default;

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsKeyRepeatImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
};
