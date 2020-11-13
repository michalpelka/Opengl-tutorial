#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aTextureId;


out vec2 TexCoord;
out vec4 v_color;
out float v_TextureId;
uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * position;
  TexCoord = aTexCoord;
  v_color = aColor;
  v_TextureId = aTextureId;
};
#shader fragment
#version 330 core
uniform vec4 u_Color;

layout(location = 0) out vec4 color; 
in vec2 TexCoord;
in vec4 v_color;
in float v_TextureId;

uniform sampler2D u_Texture[3];
void main()
{
	//color = v_color;
	//color = vec4(0,1, v_TextureId*0.4f, 1);
	int index = int( v_TextureId );
	color = texture(u_Texture[index], TexCoord);
};