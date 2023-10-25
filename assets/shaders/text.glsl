// MSDF text shader
#type vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float aProjectionID;

out vec4 vertexColor;
out vec2 texCoords;

uniform mat4 uOrthographicViewProjection;
uniform mat4 uPerspectiveViewProjection;

void main()
{
  vertexColor = aColor;
  texCoords = aTexCoords;

  if(int(aProjectionID) == 0)
  {
    gl_Position = vec4(aPos, 1.0f);
  }
  else if(int(aProjectionID) == 1)
  {
    gl_Position = uOrthographicViewProjection * vec4(aPos, 1.0f);
  }
  else if(int(aProjectionID) == 2)
  {
    gl_Position = uPerspectiveViewProjection * vec4(aPos, 1.0f);
  }
  else
  {
    gl_Position = vec4(88, 12, 42, 18);
  }
}

#type fragment
#version 460 core

out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;

layout (binding = 0) uniform sampler2D u_FontAtlas;
float screenPxRange() {
	const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(texCoords);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
void main()
{
  vec4 texColor = vertexColor * texture(u_FontAtlas, texCoords);

	vec3 msd = texture(u_FontAtlas, texCoords).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
    fragColor = mix(bgColor, vertexColor, opacity);
	if (fragColor.a == 0.0)
		discard;
}
