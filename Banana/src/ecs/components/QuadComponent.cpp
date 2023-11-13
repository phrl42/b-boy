#include "incs.h"
#include "ecs/components/QuadComponent.h"
#include "renderer/Renderer2D.h"

namespace Banana
{
  QuadComponent::QuadComponent()
  {
    this->name = "QuadComponent";
  }

  QuadComponent::QuadComponent(const std::string &name)
  {
    if(!name.empty())
    {
      tex = Texture2D::Create(name);
    }

    this->name = "QuadComponent";
  }

  void QuadComponent::OnUpdate(float dt, const Transform &transform)
  {
    if(tex != nullptr)
    {
      Renderer2D::DrawQuad(transform.pos, {transform.size.x, transform.size.y}, transform.color, transform.rotation, tex, transform.proj);
    }
    else 
    {  
      Renderer2D::DrawQuad(transform.pos, {transform.size.x, transform.size.y}, transform.color, transform.rotation, transform.proj);
    }
  }


};
