#pragma once

#include "Banana.h"
#include "renderer/Texture.h"
#include "generic/Sound.h"
#include "gbc/gbc.h"

#include <thread>

namespace SANDBOX
{
  class TestLayer : public Banana::Layer
  {
  public:
    TestLayer(const std::string& name = "Layer");
    
    ~TestLayer() override;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(float dt) override;
    virtual void OnEvent(Banana::Event& event) override;

    virtual inline std::string& GetName() override { return name; }
  
  private:
    std::string name;
    Banana::Sound sound;

    Banana::Entity ent[TFT_HEIGHT][TFT_WIDTH];

    Banana::Entity tmap1;
    Banana::Entity tmap2;
    Banana::Entity tiles;

    Banana::Entity screen;
    
    Banana::Entity play;
    Banana::Entity stop;
    Banana::Entity step;

    GBC::Spec spec;

    std::thread emu;
  };

};
