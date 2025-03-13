#include "incs.h"
#include "layer/SceneStack.h"

namespace Banana
{
  SceneStack::SceneStack()
  {
    sceneInsert = scenes.begin();
  }

  SceneStack::~SceneStack()
  {
    for(Scene* scene : scenes)
    {
      delete scene;
    }
  }

  void SceneStack::PushScene(Scene* scene)
  {
    sceneInsert = scenes.emplace(sceneInsert, scene);
  }

  void SceneStack::PopScene(Scene* scene)
  {
    auto it = std::find(scenes.begin(), scenes.end(), scene);

    if(it != scenes.end())
    {
      scenes.erase(it);
      delete *it;
      sceneInsert--;
    }
  }
  
};