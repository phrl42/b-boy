#pragma once


#include "Window.hpp"
#include "layer/Layer.h"
#include "layer/SceneStack.h"

#include "renderer/Framebuffer.h"
#include "generic/Sound.h"
namespace Banana
{
  class Application
  {
  public:
    Application();
    virtual ~Application();

    void OnEvent(Event& e);
    void Run();

    void PushScene(Scene* scene);
    
    void PopScene(Scene* scene);

    inline Window& GetWindow() { return *window; }

    inline static Application& GetInstance() { return *Instance; }
  
  private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    Layer* debug_layer;

    SceneStack scene_stack;
    std::unique_ptr<Window> window;

    static Application* Instance;

    bool running = true;
    bool minimized = false;
  public:
    Banana::Shr<Banana::Framebuffer> fb;
    // this stays as long as I do not have a better idea
    Banana::SoundHelper* soundhelper;
  };

  // definition is in client
  Application* CreateApplication();

};
