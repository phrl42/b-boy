#type vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float aProjectionID;
layout (location = 4) in float aTexID;

out vec4 vertexColor;
out vec2 texCoords;
out float texID;

uniform mat4 uOrthographicViewProjection;
uniform mat4 uPerspectiveViewProjection;

void main()
{
  if(int(aProjectionID) == 0)
  {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
  }
  else if(int(aProjectionID) == 1)
  {
    gl_Position = uOrthographicViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
  }
  else if(int(aProjectionID) == 2)
  {
    gl_Position = uPerspectiveViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
  }
  else
  {
    gl_Position = vec4(88, 12, 42, 18);
  }

  vertexColor = aColor;
  texCoords = aTexCoords;
  texID = aTexID;
}

#type fragment
#version 460 core

in vec2 texCoords;
in vec4 vertexColor;
in float texID;

out vec4 FragColor;

uniform sampler2D fTexture[32];

void main()
{
  if(int(texID) >= 0)
  {
    FragColor = texture(fTexture[int(texID)], texCoords) * vertexColor;
  }
  else
  {
    FragColor = vertexColor;
  }
}
