#include "incs.h"
#include "TextComponent.h"
#include "Font.h"
#include "renderer/Renderer2D.h"

namespace Banana
{
  TextComponent::TextComponent(const std::string &text, int font_width, int font_height)
  {
    font_texture = Font::GetDefault();
    this->name = "TextComponent";
    this->font_width = font_width;
    this->font_height = font_height;
    this->text = text;
  }

  void TextComponent::OnUpdate(float dt, const Transform &transform)
  {
    //Renderer2D::DrawQuad(transform.pos, transform.size, transform.color, transform.rotation, font_texture->GetAtlasTexture(), transform.proj);
    Renderer2D::DrawText(text, font_texture, transform.pos, transform.size, transform.color, transform.proj);
  }

  void TextComponent::ChangeText(const std::string& text)
  {
    this->text = text;  
  }

};
