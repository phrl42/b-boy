#pragma once
#include "Projection.h"

#include "renderer/Texture.h"

#include "Component.h"

namespace Banana
{
  class Entity
  {
  public:
    Entity()
    : transform(Transform())
    {

    }

    Entity(const Transform &transform)
    :transform(transform)
    {

    }

    inline virtual ~Entity()
    {
      for(Component* comp : components)
      {
        delete comp;
      }
    }

    void AddComponent(Component* component) 
    {
      for(Component* comp : components)
      {
        if(comp->GetName() == component->GetName())
        {
          return;
        }
      }
      components.push_back(component);
    }

    void RemoveComponent(const std::string& name)
    {
      Component* comp_searched;
      for(Component* comp : components)
      {
        if(comp->GetName() == name)
        {
          comp_searched = comp;
          break;
        }
        LOG("To-removed component does not exist");
        return;
      }

      std::vector<Component*>::iterator it = std::find(components.begin(), components.end(), comp_searched);

      if(it != components.end())
      {
        components.erase(it);
        delete *it;
      }
      else 
      {
        LOG("Could not find found component");
        LOG("This should not happen");
      }
    }

    Component* GetComponent(const std::string& name)
    {
      for(Component* comp : components)
      {
        if(comp->GetName() == name)
        {
          return comp;
        }
      }
      LOG("Could not find component");
      return nullptr;
    }

    void Render(float dt)
    {
      for(Component* comp : components)
      {
        comp->OnUpdate(dt, transform);
      }
    }

  public:
    Transform transform;

  private:
    std::vector<Component*> components;
  };
};