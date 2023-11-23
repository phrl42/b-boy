#include "scene/layer/TestLayer.h"

#include "renderer/Renderer2D.h"

#include "ecs/components/QuadComponent.h"
#include "ecs/components/TextComponent.h"
#include "ecs/components/TileComponent.h"

#include "imgui_debug.h"

namespace SANDBOX
{

  TestLayer::TestLayer(const std::string& name)
    : name(name), sound(Banana::Sound("assets/sounds/test.wav", false))
  {
    play.AddComponent(new Banana::QuadComponent("assets/textures/play.png"));
    stop.AddComponent(new Banana::QuadComponent("assets/textures/stop.png"));
    step.AddComponent(new Banana::QuadComponent("assets/textures/step.png"));

    Banana::QuadComponent *qcp = (Banana::QuadComponent*)play.GetComponent("QuadComponent");
    Banana::QuadComponent *qcs = (Banana::QuadComponent*)stop.GetComponent("QuadComponent");
    Banana::QuadComponent *qct = (Banana::QuadComponent*)step.GetComponent("QuadComponent");

    Stats::play_id = qcp->GetTextureID();
    Stats::stop_id = qcs->GetTextureID();
    Stats::step_id = qct->GetTextureID();

    spec.Init("assets/roms/cpu_instrs.gb");
    Stats::spec = &spec;

    tilemap.AddComponent(new Banana::TileComponent(spec.ppu.tile, 256));

    tilemap.transform.proj = Banana::Projection::PERSPECTIVE;

    Banana::TileComponent *tc = (Banana::TileComponent*)tilemap.GetComponent("TileComponent");
    Stats::tiles_id = tc->GetTextureID();
   }

  TestLayer::~TestLayer()
  {

  }

  void TestLayer::OnAttach()
  {
    for(size_t y = 0; y < TFT_HEIGHT; y++)
    {
      for(size_t x = 0; x < TFT_WIDTH; x++)
      {
	ent[y][x].AddComponent(new Banana::QuadComponent());
      }
    }
 }

  void TestLayer::OnDetach()
  {

  }


  void TestLayer::OnEvent(Banana::Event& event)
  {

  }

  void TestLayer::OnUpdate(float dt)
  {
    float one_width = 2.0f / TFT_WIDTH;
    float one_height = 2.0f / TFT_HEIGHT;
    for(size_t y = 0; y < TFT_HEIGHT; y++)
    {
      for(size_t x = 0; x < TFT_WIDTH; x++)
      {
	
	//ent[y][x].transform.proj = Banana::Projection::NONE;
	//ent[y][x].transform.pos = {(x * one_width) - 1, (((y * one_height) * -1) + 1) - one_height, 0};
	//ent[y][x].transform.size = {one_width, one_height, 0};
	//ent[y][x].transform.color = {spec.gpu.display[y][x], spec.gpu.display[y][x], spec.gpu.display[y][x], 1};
	//ent[y][x].Render(dt);
      }
    }

    Banana::TileComponent *tc = (Banana::TileComponent*)tilemap.GetComponent("TileComponent");
    tc->UpdateTileData();

    spec.Update();
  }
};
