#include "incs.h"
#include "renderer/Renderer2D.h"

#include "renderer/VertexArray.h"
#include "renderer/Shader.h"

#include "renderer/RenderCommand.hpp"

#include "renderer/Texture.h"
#include "renderer/MSDFData.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

namespace Banana
{
  struct QuadVertex
  {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coords;
    float projID;
    float texID;
  };

  struct TextVertex
  {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coords;
    float projID;
  };

  struct Renderer2DStorage
  {
    static const uint32_t MAX_QUADS = 20000;
    static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
    static const uint32_t MAX_INDICES = MAX_QUADS * 6;
		static const uint32_t MAX_TEXTURE_SLOTS = 32;

    Shr<VertexArray> quad_vertex_array;
    Shr<VertexBuffer> quad_vertex_buffer;
    Shr<Shader> quad_shader;

    Shr<VertexArray> text_vertex_array;
    Shr<VertexBuffer> text_vertex_buffer;
    Shr<Shader> text_shader;

    Camera scene_camera;

    // quad batch
    uint32_t QuadIndexCount = 0;
    QuadVertex* quad_vertex_base = nullptr;
    QuadVertex* quad_vertex_ptr = nullptr;

    // texture batch
    uint32_t TextIndexCount = 0;
    TextVertex* text_vertex_base = nullptr;
    TextVertex* text_vertex_ptr = nullptr;

		std::array<Shr<Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
    
    Shr<Texture2D> FontAtlasTexture;
  };

  static Renderer2DStorage data;

  void Renderer2D::Init()
  {
    // QUAD API STUFF
    data.quad_vertex_array = VertexArray::Create();

    data.quad_vertex_buffer = VertexBuffer::Create(
    {
      {ShaderDataType::Float3, "aPos"},
      {ShaderDataType::Float4, "aColor"},
      {ShaderDataType::Float2, "aTexCoords"},
      {ShaderDataType::Float, "aProjectionID"},
      {ShaderDataType::Float, "aTexID"}
    }, sizeof(QuadVertex) * data.MAX_QUADS);

    data.quad_vertex_array->AddVertexBuffer(data.quad_vertex_buffer);
  
    data.quad_vertex_base = new QuadVertex[data.MAX_VERTICES];

    uint32_t *quad_indices = new uint32_t[data.MAX_INDICES];

    uint32_t offset = 0;
    for(int i = 0; i < data.MAX_INDICES; i += 6)
    {
      quad_indices[i + 0] = offset;
      quad_indices[i + 1] = offset + 1;
      quad_indices[i + 2] = offset + 2;

      quad_indices[i + 3] = offset + 1;
      quad_indices[i + 4] = offset + 2;
      quad_indices[i + 5] = offset + 3;

      offset += 4;
    }

    Shr<IndexBuffer> quad_index_buffer = IndexBuffer::Create(quad_indices, data.MAX_INDICES);
    data.quad_vertex_array->SetIndexBuffer(quad_index_buffer);

    data.quad_shader = Shader::Create("assets/shaders/quad.glsl");
    // maybe merge compile into create
    data.quad_shader->Compile();
    
    // text api stuff
    data.text_vertex_array = VertexArray::Create();
    data.text_vertex_buffer = VertexBuffer::Create(
    {
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float4, "aColor"},
        {ShaderDataType::Float2, "aTexCoords"},
        {ShaderDataType::Float, "aProjectionID"}
    }, sizeof(TextVertex) * data.MAX_VERTICES);

    data.text_vertex_array->AddVertexBuffer(data.text_vertex_buffer);
    data.text_vertex_base = new TextVertex[data.MAX_VERTICES];

    data.text_vertex_array->SetIndexBuffer(quad_index_buffer);
    delete[] quad_indices;
    
    data.text_shader = Shader::Create("assets/shaders/text.glsl");
    data.text_shader->Compile();

    // uniform stuff
    int32_t samplers[data.MAX_TEXTURE_SLOTS];
    for (uint32_t i = 0; i < data.MAX_TEXTURE_SLOTS; i++)
      samplers[i] = i;
    data.quad_shader->UploadIntArray("fTexture", data.MAX_TEXTURE_SLOTS, samplers);
  }

  void Renderer2D::Shutdown()
  {
    delete[] data.quad_vertex_base;
    delete[] data.text_vertex_base;
  }

  void Renderer2D::BeginScene(const Camera& cam)
  {
    data.scene_camera = cam;
    RenderCommand::Clear();
    StartBatch();
  }

  void Renderer2D::BeginScene()
  {
    LOG("Warning! Client did not specify a camera");
    data.scene_camera = Camera();
    RenderCommand::Clear();
    StartBatch();
  }

  void Renderer2D::StartBatch()
  {
    data.quad_vertex_ptr = data.quad_vertex_base;
    data.QuadIndexCount = 0;

    data.TextureSlotIndex = 0;

    data.text_vertex_ptr = data.text_vertex_base;
    data.TextIndexCount = 0;

    // upload uniforms here
    data.quad_shader->UploadMat4f("uPerspectiveViewProjection", data.scene_camera.GetPerspectiveViewProjection());
    data.quad_shader->UploadMat4f("uOrthographicViewProjection", data.scene_camera.GetOrthographicViewProjection());

    //data.text_shader->UploadMat4f("uPerspectiveViewProjection", data.scene_camera.GetPerspectiveViewProjection());
    //data.text_shader->UploadMat4f("uOrthographicViewProjection", data.scene_camera.GetOrthographicViewProjection());
  }

  void Renderer2D::NextBatch()
  {
    Flush();
    StartBatch();
  }

  void Renderer2D::EndScene()
  {
    Flush();
  }

  void Renderer2D::Flush()
  {
    if(data.QuadIndexCount)
    {
      uint32_t data_size = (uint32_t)((uint8_t*)data.quad_vertex_ptr - (uint8_t*)data.quad_vertex_base);
      data.quad_vertex_buffer->SetData(data.quad_vertex_base, data_size);

      for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
        data.TextureSlots[i]->Bind(i);

      data.quad_shader->Bind();
      RenderCommand::DrawIndexed(data.quad_vertex_array, data.QuadIndexCount);
    }

    if(data.TextIndexCount)
    {
      uint32_t data_size = (uint32_t)((uint8_t*)data.text_vertex_ptr - (uint8_t*)data.text_vertex_base);
      data.text_vertex_buffer->SetData(data.text_vertex_base, data_size);
      
      data.FontAtlasTexture->Bind(0);

      data.text_shader->Bind();
      RenderCommand::DrawIndexed(data.text_vertex_array, data.TextIndexCount);
    }
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, const Shr<Texture2D>& texture, Projection proj)
  {
    if(texture == nullptr)
    {
      LOG("Omitted nullptr texture in renderer: Please initialize your texture");
      return;
    }

    if (data.QuadIndexCount >= data.MAX_INDICES)
      NextBatch();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
    {
      if (*data.TextureSlots[i] == *texture)
      {
        textureIndex = (float)i;
        break;
      }
    }

    if (textureIndex == 0.0f)
    {
      if(data.TextureSlotIndex >= data.MAX_TEXTURE_SLOTS)
        NextBatch();

      textureIndex = (float)data.TextureSlotIndex;
      data.TextureSlots[data.TextureSlotIndex] = texture;
      data.TextureSlotIndex++;
    }

    // bottom left
    data.quad_vertex_ptr->position = glm::vec4({pos, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0, 0};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    // bottom right
    data.quad_vertex_ptr->position = glm::vec4({pos.x + size.x, pos.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1.0f, 0.0f};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    // top left
    data.quad_vertex_ptr->position = glm::vec4({pos.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0.0f, 1.0f};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    // top right
    data.quad_vertex_ptr->position = glm::vec4({pos.x + size.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1.0f, 1.0f};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    data.QuadIndexCount += 6;
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rotation, const Shr<Texture2D>& texture, Projection proj)
  {
    if(rotation == 0)
    {
      DrawQuad(pos, size, color, texture, proj);
      return;
    }
    if(texture == nullptr)
    {
      LOG("Omitted nullptr texture in renderer: Please initialize your texture");
      return;
    }

    if (data.QuadIndexCount >= data.MAX_INDICES)
      NextBatch();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
    {
      if (*data.TextureSlots[i] == *texture)
      {
        textureIndex = (float)i;
        break;
      }
    }

    if (textureIndex == 0.0f)
    {
      if(data.TextureSlotIndex >= data.MAX_TEXTURE_SLOTS)
        NextBatch();

      textureIndex = (float)data.TextureSlotIndex;
      data.TextureSlots[data.TextureSlotIndex] = texture;
      data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
      * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
      * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    // bottom left
    data.quad_vertex_ptr->position = transform * glm::vec4({pos, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0, 0};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    // bottom right
    data.quad_vertex_ptr->position = transform * glm::vec4({pos.x + size.x, pos.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1.0f, 0.0f};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    // top left
    data.quad_vertex_ptr->position = transform * glm::vec4({pos.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0.0f, 1.0f};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    // top right
    data.quad_vertex_ptr->position = transform * glm::vec4({pos.x + size.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1.0f, 1.0f};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = textureIndex;
    data.quad_vertex_ptr++;

    data.QuadIndexCount += 6;
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, Projection proj)
  {
    if (data.QuadIndexCount >= data.MAX_INDICES)
    {
      NextBatch();
    }

    // bottom left
    data.quad_vertex_ptr->position = glm::vec4({pos, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0, 0};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    // bottom right
    data.quad_vertex_ptr->position = glm::vec4({pos.x + size.x, pos.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1, 0};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    // top left
    data.quad_vertex_ptr->position = glm::vec4({pos.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0, 1};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    // top right
    data.quad_vertex_ptr->position = glm::vec4({pos.x + size.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1, 1};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    data.QuadIndexCount += 6;
  } 

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rotation, Projection proj)
  {
    if(rotation == 0)
    {
      DrawQuad(pos, size, color, proj);
      return;
    }

    if (data.QuadIndexCount >= data.MAX_INDICES)
    {
      NextBatch();
    }
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
      * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
      * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    // bottom left
    data.quad_vertex_ptr->position = transform * glm::vec4({pos, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0, 0};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    // bottom right
    data.quad_vertex_ptr->position = transform * glm::vec4({pos.x + size.x, pos.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1, 0};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    // top left
    data.quad_vertex_ptr->position = transform * glm::vec4({pos.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {0, 1};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    // top right
    data.quad_vertex_ptr->position = transform * glm::vec4({pos.x + size.x, pos.y + size.y, pos.z, 1.0f});
    data.quad_vertex_ptr->color = color;
    data.quad_vertex_ptr->tex_coords = {1, 1};
    data.quad_vertex_ptr->projID = proj;
    data.quad_vertex_ptr->texID = -1;
    data.quad_vertex_ptr++;

    data.QuadIndexCount += 6;
  }

  void Renderer2D::DrawText(const std::string& text, Shr<Font> font, const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color, Projection proj)
  {
		const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Shr<Texture2D> fontAtlas = font->GetAtlasTexture();

		data.FontAtlasTexture = fontAtlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;

		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < text.size(); i++)
    {
			char character = text[i];
			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight + 0;
				continue;
			}

			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < text.size() - 1)
				{
					char nextCharacter = text[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + 0;
				continue;
			}

			if (character == '\t')
			{
				// NOTE(Yan): is this right?
				x += 4.0f * (fsScale * spaceGlyphAdvance + 0);
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				return;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

      
      // render here
      data.text_vertex_ptr->position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
      data.text_vertex_ptr->color = color;
      data.text_vertex_ptr->tex_coords = texCoordMin;
      data.text_vertex_ptr->projID = proj;
      data.text_vertex_ptr++;

      data.text_vertex_ptr->position = transform * glm::vec4(quadMax.x, 0.0f, 0.0f, 1.0f);
      data.text_vertex_ptr->color = color;
      data.text_vertex_ptr->tex_coords = { texCoordMax.x, texCoordMin.y};  
      data.text_vertex_ptr->projID = proj;
      data.text_vertex_ptr++;

      data.text_vertex_ptr->position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0, 1.0f);
      data.text_vertex_ptr->color = color;
      data.text_vertex_ptr->tex_coords = { texCoordMin.x, texCoordMax.y }; 
      data.text_vertex_ptr->projID = proj;
      data.text_vertex_ptr++;

      data.text_vertex_ptr->position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
      data.text_vertex_ptr->color = color;
      data.text_vertex_ptr->tex_coords = texCoordMax;
      data.text_vertex_ptr->projID = proj;
      data.text_vertex_ptr++;

      data.TextIndexCount += 6;

      if (i < text.size() - 1)
      {
        double advance = glyph->getAdvance();
        char nextCharacter = text[i + 1];
        fontGeometry.getAdvance(advance, character, nextCharacter);

        x += fsScale * advance + 0;
      }
    }
  }

};
