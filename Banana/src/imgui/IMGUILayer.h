#pragma once

#include "layer/Layer.h"

namespace Banana
{
  class IMGUILayer : public Layer
  {
  public:
    IMGUILayer(const std::string& name = "IMGUILayer");

    virtual ~IMGUILayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(float dt) override;
    virtual void OnEvent(Event& event) override;

    virtual inline std::string& GetName() override { return name; }

  private:
    std::string name;
    float l_time;
  };
};