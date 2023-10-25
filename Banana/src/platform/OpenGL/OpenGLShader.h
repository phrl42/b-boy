#pragma once
#include "renderer/Shader.h"


namespace Banana
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& file_path);

		virtual ~OpenGLShader() override = default;
		virtual void Compile() override;
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void UploadMat4f(const char* varName, glm::mat4 mat4) override;
		virtual void UploadMat3f(const char* varName, glm::mat3 mat3) override;
		virtual void UploadVec4f(const char* varName, glm::vec4 vec4) override;
		virtual void UploadVec3f(const char* varName, glm::vec3 vec3) override;
		virtual void UploadVec2f(const char* varName, glm::vec2 vec2) override;
		virtual void UploadFloat(const char* varName, float value) override;
		virtual void UploadInt(const char* varName, int value) override;
		virtual void UploadIntArray(const char* varName, int arrayLength, int array[]) override;
		virtual void UploadTexture(const char* varName, int slot) override;

	private:
		uint32_t id;
		const std::string file_path;

		std::string vertex_src = "";
		std::string fragment_src = "";
	};
};
