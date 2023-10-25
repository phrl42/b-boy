#pragma once
#include "Window.hpp"
#include "renderer/Context.hpp"

struct GLFWwindow;

namespace Banana
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowProps& window_props);
		~GLFWWindow() override;

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual unsigned int GetHeight() override;
		virtual unsigned int GetWidth() override;

		virtual double GetTime() override;

		virtual void Resize(unsigned int width, unsigned int height) override;

		virtual void SetEventCallback(const EventCallbackFunction& callback) override
		{
			windowData.callback = callback;
		}

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
		virtual void EnableFullscreen(bool enabled) override;
		virtual void* GetNativeWindow() const override;

	private:
		Context* context;
		GLFWwindow* glfwWindow;

		bool fullscreenEnabled = false;
		bool initialized = false;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vsync;
			EventCallbackFunction callback;
		};

		WindowData windowData;
		void Init(const WindowProps& window_props);
		void Quit() const;
	};
};
