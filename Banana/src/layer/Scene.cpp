#include "incs.h"
#include "layer/Scene.h"

#include "renderer/Renderer2D.h"

namespace Banana
{
  Scene::Scene(const std::string& name)
  : name(name), cam(Camera()) {}


  void Scene::PopLayer(Layer* layer)
  {
    layer_stack.PopLayer(layer);
  }

  void Scene::PushLayer(Layer* layer)
  {
    layer_stack.PushLayer(layer);
  }

  void Scene::PopOverlay(Layer* layer)
  {
    layer_stack.PopOverlay(layer);
  }

  void Scene::PushOverlay(Layer* layer)
  {
    layer_stack.PushOverlay(layer);
  }

  void Scene::OnEvent(Event& e)
  {
    Banana::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Banana::WindowResizeEvent>(BIND_EVENT_FN(Scene::OnWindowResize));

    for(auto it = layer_stack.end(); it != layer_stack.begin();)
    {
      (*--it)->OnEvent(e);

      if(e.handled)
      {
        break;
      }
    }
  }

  bool Scene::OnWindowResize(Banana::WindowResizeEvent& e)
  {
    cam.SetWindowDimension(e.getWidth(), e.getWidth());
    return false;
  }

  void Scene::AttachLayer()
  {
    for(Layer* layer : layer_stack)
    {
      layer->OnAttach();
    }
  }

  void Scene::RenderLayer(float dt)
  {
    Banana::Renderer2D::BeginScene(cam);
    for(auto layer : layer_stack)
    {
      layer->OnUpdate(dt);
    }
    Banana::Renderer2D::EndScene();
  }

  void Scene::DetachLayer()
  {
    for(Layer* layer : layer_stack)
    {
      layer->OnDetach();
    }
  }

};