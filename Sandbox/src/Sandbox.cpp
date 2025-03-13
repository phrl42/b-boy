#include "Sandbox.h"
#include "_Sandbox.h"

#include "scene/layer/TestLayer.h"
#include "scene/EntryScene.h"

//-----EntryPoint.h-----
#include "EntryPoint.h"

namespace SANDBOX
{

  Sandbox::Sandbox()
  {
    //PushLayer(new TestLayer("Test"));
    PushScene(new EntryScene("Entry"));
  }

  Sandbox::~Sandbox()
  {

  }

};

Banana::Application* Banana::CreateApplication()
{
  return new SANDBOX::Sandbox();
}