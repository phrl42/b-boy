#include "incs.h"
#include "Font.h"

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"

#include "renderer/MSDFData.h"

namespace Banana
{
  	
  template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Shr<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
		const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

    TextureSpecification spec;
    spec.width = bitmap.width;
    spec.height = bitmap.height;
    spec.format = ImageFormat::RGB8;
    spec.GenerateMips = false;
    spec.data = (void*)bitmap.pixels;
    spec.size = bitmap.width * bitmap.height * 3;

    Shr<Texture2D> texture = Texture2D::Create(spec);

		return texture;
	}

  Font::Font(const std::string &font_path)
  : data(new MSDFData())
  {		
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();

		msdfgen::FontHandle* font = msdfgen::loadFont(ft, font_path.c_str());

    if(!font)
    {
      LOG("Font file could not be found: " + font_path);
    }

		struct CharsetRange
		{
			uint32_t Begin, End;
		};

		// From imgui_draw.cpp
		static const CharsetRange charsetRanges[] =
		{
			{ 0x0020, 0x00FF }
		};

		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges)
		{
			for (uint32_t c = range.Begin; c <= range.End; c++)
				charset.add(c);
		}
    
    double fontScale = 1.0;
		data->FontGeometry = msdf_atlas::FontGeometry(&data->Glyphs);
		int glyphsLoaded = data->FontGeometry.loadCharset(font, fontScale, charset);
	  LOG("Loaded " + std::to_string(glyphsLoaded) + " glyphs from font (out of " + std::to_string(charset.size()) + ") ");

    double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		// atlasPacker.setDimensionsConstraint()
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);
		int remaining = atlasPacker.pack(data->Glyphs.data(), (int)data->Glyphs.size());
    
		if(remaining)
    {
      LOG("Remainigs are " + std::to_string(remaining));
    }

		int width = 0, height = 0;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
		// if MSDF || MTSDF
		
    uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if (expensiveColoring)
		{
			msdf_atlas::Workload([&glyphs = data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
				}, data->Glyphs.size()).finish(THREAD_COUNT);
		}
		else {
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : data->Glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}


		atlas_texture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, data->Glyphs, data->FontGeometry, width, height);

    // save glyph to fs

#if 1
		msdfgen::Shape shape;
		if (msdfgen::loadGlyph(shape, font, 'C'))
		{
			shape.normalize();
			//                      max. angle
			msdfgen::edgeColoringSimple(shape, 3.0);
			//           image width, height
			msdfgen::Bitmap<float, 3> msdf(32, 32);
			//                     range, scale, translation
			msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
			msdfgen::savePng(msdf, "output.png");
		}
#endif

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
  }
  
  Font::~Font()
  {
    delete data;
  }

  Shr<Font> Font::GetDefault()
  {
    static Shr<Font> font;
    if(!font)
    {
      font = std::make_shared<Font>("assets/fonts/mononoki.ttf");
    }

    return font;
  }

};
