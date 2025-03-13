#pragma once

#include "event/ApplicationEvent.h"
#include "generic/Camera.h"
#include "layer/LayerStack.h"

namespace Banana
{
	// TODO: in the future:
	// implement something like a Scene2D / Scene3D system
	// for inspiration look at the texture class
	class Scene
	{
	public:
		Scene(const std::string& name = "Scene");

		virtual ~Scene() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void OnUpdate(float dt) = 0;
		virtual void OnEvent(Event& e);

		bool OnWindowResize(Banana::WindowResizeEvent& e);

		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		void PopLayer(Layer* layer);

		void AttachLayer();

		void RenderLayer(float dt);

		void DetachLayer();

	protected:
		std::string name;
		Camera cam;

		LayerStack layer_stack;
	};
};
