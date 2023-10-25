#pragma once

namespace Banana
{
	class Input
	{
	public:
		Input() = default;
		virtual ~Input() = default;

		static bool IsKeyPressed(int key) { return Instance->IsKeyPressedImpl(key); }
		static bool IsKeyRepeat(int key) { return Instance->IsKeyRepeatImpl(key); }

		static bool IsMouseButtonPressed(int button) { return Instance->IsMouseButtonPressedImpl(button); }
		static float GetMouseX() { return Instance->GetMouseXImpl(); }
		static float GetMouseY() { return Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsKeyRepeatImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;

		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* Instance;
	};
};
