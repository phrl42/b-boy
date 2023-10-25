
#include "layer/Scene.h"

namespace Banana
{
  class SceneStack
  {
  public:
    SceneStack();
    ~SceneStack();

    void PushScene(Scene* scene);
    void PopScene(Scene* scene);

    std::vector<Scene*>::iterator begin() { return scenes.begin(); }
    std::vector<Scene*>::iterator end() { return scenes.end(); }

  private:
    std::vector<Scene*> scenes;
    std::vector<Scene*>::iterator sceneInsert;
  };
};