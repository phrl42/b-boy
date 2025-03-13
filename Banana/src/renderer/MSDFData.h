#include "Font.h"

#include <vector>

#include "msdf-atlas-gen.h"

namespace Banana
{
  struct MSDFData
  {
    std::vector<msdf_atlas::GlyphGeometry> Glyphs;
    msdf_atlas::FontGeometry FontGeometry;
  };
};