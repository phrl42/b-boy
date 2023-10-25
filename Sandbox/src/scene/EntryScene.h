#pragma once

#include "Banana.h"
#include "Sandbox.h"

namespace SANDBOX
{
  class EntryScene : public Banana::Scene
  {
  public:
    EntryScene(const std::string& name);

    ~EntryScene() override = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(float dt) override;

  private:
    std::string name;
  };
};