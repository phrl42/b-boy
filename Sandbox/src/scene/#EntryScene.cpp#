 #include "scene/EntryScene.h"
#include "scene/layer/TestLayer.h"
#include "layer/Layer.h"

#include "renderer/Renderer2D.h"

namespace SANDBOX
{
  EntryScene::EntryScene(const std::string& name)
  :name(name)
  {
    layer_stack.PushLayer(new TestLayer("Layer One"));
  }

  void EntryScene::OnAttach()
  {
    AttachLayer();
  }

  void EntryScene::OnUpdate(float dt)
  {
    static float x = 0;
    static float y = 0;
    static float z = 1;
    
    if(Banana::Input::IsKeyPressed(KEY_S))
    {
      y -= 10 * dt;
    }
    
    if(Banana::Input::IsKeyPressed(KEY_W))
    {
      y += 10 * dt;
    }
    
    if(Banana::Input::IsKeyPressed(KEY_A))
    {
      x -= 10 * dt;
    }

    if(Banana::Input::IsKeyPressed(KEY_D))
    {
      x += 10 * dt;
    }

    if(Banana::Input::IsKeyPressed(KEY_T))
    {
      z += 10 * dt;
    }

    if(Banana::Input::IsKeyPressed(KEY_G))
    {
      z -= 10 * dt;
    }

    cam.SetPosition({x, y, z});

    RenderLayer(dt);
  }

  void EntryScene::OnDetach()
  {
    DetachLayer();
  }

};
