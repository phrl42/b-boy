#pragma once


#include "layer/Layer.h"

namespace Banana
{
  class LayerStack
  {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopOverlay(Layer* overlay);
    void PopLayer(Layer* layer);

    std::vector<Layer*>::iterator begin() { return layers.begin(); }
    std::vector<Layer*>::iterator end() { return layers.end(); }

  private:
    std::vector<Layer*> layers;
    std::vector<Layer*>::iterator layerInsert;

  };
};