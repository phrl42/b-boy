#pragma once
#include "renderer/Texture.h"

namespace Banana
{
  struct MSDFData;

  class Font
  {
  public:
    ~Font();
    
    Font(const std::string& font_path);

    static Shr<Font> GetDefault();

    Shr<Texture2D> GetAtlasTexture() const { return atlas_texture; }

    const MSDFData* GetMSDFData() const { return data;}

  private:
    MSDFData* data;
    Shr<Texture2D> atlas_texture;

  };
};
