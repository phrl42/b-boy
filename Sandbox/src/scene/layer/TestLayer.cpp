#include "scene/layer/TestLayer.h"

#include "renderer/Renderer2D.h"

#include "ecs/components/QuadComponent.h"
#include "ecs/components/TextComponent.h"

#include "imgui_debug.h"

namespace SANDBOX
{

  std::ofstream file;
  TestLayer::TestLayer(const std::string& name)
    : name(name), sound(Banana::Sound("assets/sounds/test.wav", false))
  {
    GBC::Init_Spec(&spec, "assets/roms/Tetris.gb");
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

    Stats::PC = spec.PC;
    Stats::SP = spec.SP;
    Stats::AF = spec.AF;
    Stats::BC = spec.BC;
    Stats::DE = spec.DE;
    Stats::HL = spec.HL;
    Stats::opval = spec.ram[spec.PC];
    GBC::Update(&spec);

  }
};
