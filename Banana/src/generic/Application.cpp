#include "incs.h"
#include "Application.hpp"

#include "imgui/IMGUILayer.h"
#include "event/Input.hpp"
#include "event/KeyCode.h"

#include "renderer/Renderer.h"

#include "renderer/RenderCommand.hpp"

#include "Font.h"

namespace Banana
{

  Application* Application::Instance = nullptr;

  Application::Application()
  {
    if(Instance)
    {
      LOG("Application already exists!");
    }

    Instance = this;

    WindowProps prop;
    prop.Title = "Banana Engine";
    prop.height = 1000;
    prop.width = 1600;

    window = Window::Create(prop);

    window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    Renderer::Init();

    Banana::FramebufferProperties spec;
    spec.width = prop.height;
    spec.height = prop.width;

    fb = Banana::Framebuffer::Create(spec);
    fb_tiles = Banana::Framebuffer::Create(spec);

    debug_layer = new Banana::IMGUILayer("IMGUILAYER");
    
    soundhelper = new SoundHelper();
    //Text::Init("assets/fonts/mononoki.ttf");
  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

    for(auto it = scene_stack.end(); it != scene_stack.begin();)
    {
      (*--it)->OnEvent(e);

      if(e.handled)
      {
        break;
      }
    }
  }

  void Application::PopScene(Scene* scene)
  {
    scene_stack.PopScene(scene);
  }

  void Application::PushScene(Scene* scene)
  {
    scene_stack.PushScene(scene);
  }

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    running = false;
    return true;
  }
  
  bool Application::OnWindowResize(WindowResizeEvent& e)
  {
    if(!e.getWidth() || !e.getHeight()) 
    {
      minimized = true;
      return false;
    }
    
    // tell opengl to resize framebuffer
    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
    fb->SetWindowDimension(e.getWidth(), e.getHeight());

    minimized = false;
    return false;
  }


  void Application::Run()
  {
    for(Scene* scene : scene_stack)
    {
      scene->OnAttach();
    }

    debug_layer->OnAttach();

    double begin_time = 0.0f;
    double dt = 0.1f;

    window->SetVSync(1);
    
    while(running)
    {
      begin_time = window->GetTime();
      window->PollEvents();

      // todo: come up with a better framebuffer implementation ( probably in the scenes )
      fb->Bind();
      RenderCommand::SetClearColor(glm::vec4(1, -0.02, 0.5, 0));

      if(!minimized)
      { 
        // toggling
        static bool press = false;
        static bool debug = true;

        if(Input::IsKeyPressed(KEY_U) && !press)
        {
          press = true;
          debug = !debug;
        }

        if(!Input::IsKeyPressed(KEY_U))
        {
          press = false;
        }

        if(debug)
        {
          fb->Unbind();
          debug_layer->OnUpdate(dt);
          fb->Bind();
        }
        else 
        {
          RenderCommand::CopyFramebuffer(fb->GetID(), 0, GetWindow().GetWidth(), GetWindow().GetHeight());
        }
        
        if(!debug)
        {

        }

	for(Scene* scene : scene_stack)
	{
	  scene->OnUpdate(dt);
	}

      }
      
      fb->Unbind();
      
      window->SwapBuffers();
      RenderCommand::Clear();

      dt = window->GetTime() - begin_time;
    }
    
    for(Scene* scene : scene_stack)
    {
      scene->OnDetach();
    }

    debug_layer->OnDetach();

    Renderer::Shutdown();
  }

  Application::~Application()
  {
    delete soundhelper;
  }

};
