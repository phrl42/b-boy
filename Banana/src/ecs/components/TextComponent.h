#pragma once

#include "ecs/Component.h"
#include "generic/Font.h"

namespace Banana
{
  class TextComponent : public Component
  {
  public:
    virtual ~TextComponent() = default;

    TextComponent(const std::string& text, int font_width = 1024, int font_height = 1024);

    virtual void OnUpdate(float dt, const Transform& transform) override;

    void ChangeText(const std::string& text);
    
  private:
    Shr<Font> font_texture;
    int font_width;
    int font_height;
    std::string text;
  };
};
