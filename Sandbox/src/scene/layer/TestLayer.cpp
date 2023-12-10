#include "scene/layer/TestLayer.h"

#include "renderer/Renderer2D.h"

#include "ecs/components/QuadComponent.h"
#include "ecs/components/TextComponent.h"
#include "ecs/components/TileComponent.h"
#include "ecs/components/LineComponent.h"

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

    spec.Init("assets/roms/dmg-acid2.gb");
    Stats::spec = &spec;

    screen.AddComponent(new Banana::LineComponent(&spec.ppu.screen));
    Banana::LineComponent *lc = (Banana::LineComponent*)screen.GetComponent("LineComponent");
    
    tmap1.AddComponent(new Banana::TileComponent(spec.ppu.tmap1, 32*32));
    Banana::TileComponent *tc = (Banana::TileComponent*)tmap1.GetComponent("TileComponent");

    tmap2.AddComponent(new Banana::TileComponent(spec.ppu.tmap2, 32*32));
    Banana::TileComponent *tct = (Banana::TileComponent*)tmap2.GetComponent("TileComponent");
    
    tiles.AddComponent(new Banana::TileComponent(spec.ppu.tile, 200));
    Banana::TileComponent *tilest = (Banana::TileComponent*)tiles.GetComponent("TileComponent");

    Stats::tmap1_id = tc->GetTextureID();
    Stats::tmap2_id = tct->GetTextureID();
    Stats::tiles_id = tilest->GetTextureID();
    Stats::screen_id = lc->GetTextureID();
  }

  TestLayer::~TestLayer()
  {
    emu_kill = true;
    emu.join();
  }

  void TestLayer::OnAttach()
  {
    for(size_t y = 0; y < TFT_HEIGHT; y++)
    {
      for(size_t x = 0; x < TFT_WIDTH; x++)
      {
	//ent[y][x].AddComponent(new Banana::QuadComponent());
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
    static bool first = true;
    if(first)
    {
      first = false;
      emu = std::thread(&TestLayer::UpdateEmu, this);
    }

    Banana::LineComponent *screent = (Banana::LineComponent*)screen.GetComponent("LineComponent");
    Banana::TileComponent *tc = (Banana::TileComponent*)tmap1.GetComponent("TileComponent");
    Banana::TileComponent *tct = (Banana::TileComponent*)tmap2.GetComponent("TileComponent");
    Banana::TileComponent *tctiles = (Banana::TileComponent*)tiles.GetComponent("TileComponent");
    
    tc->UpdateTileData();
    tct->UpdateTileData();
    tctiles->UpdateTileData();
    screent->UpdateTileData();

    spec.ppu.UpdateMaps();
    spec.ppu.UpdateTiles();
  }

  void TestLayer::UpdateEmu()
  {
    while(!emu_kill)
    {
      spec.Update();
    } 
  }
};
