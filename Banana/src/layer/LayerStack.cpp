#include "incs.h"
#include "layer/LayerStack.h"

namespace Banana
{
  LayerStack::LayerStack()
  {
    layerInsert = layers.begin();
  }

  LayerStack::~LayerStack()
  {
    for(Layer* layer : layers)
    {
      delete layer;
    }
  }

  void LayerStack::PopLayer(Layer* layer)
  {
    auto it = std::find(layers.begin(), layers.end(), layer);

    if(it != layers.end())
    {
      layers.erase(it);
      delete *it;
      layerInsert--;
    }
  }
  
  void LayerStack::PushLayer(Layer* layer)
  {
    layerInsert = layers.emplace(layerInsert, layer);
  }

  void LayerStack::PopOverlay(Layer* overlay)
  {
    auto it = std::find(layers.begin(), layers.end(), overlay);

    if(it != layers.end())
    {
      layers.erase(it);
      delete *it;
      layerInsert--;
    }
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
    layers.emplace_back(overlay);
  }

};