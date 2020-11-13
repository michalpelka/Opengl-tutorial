#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;


out vec2 TexCoord;
out vec4 v_color;
uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * position;
  TexCoord = aTexCoord;
  v_color = aColor;
};
#shader fragment
#version 330 core
uniform vec4 u_Color;

layout(location = 0) out vec4 color; 
in vec2 TexCoord;
in vec4 v_color;

uniform sampler2D u_Texture;
void main()
{
	color = v_color;
	//color = u_Color;
	//color = texture(u_Texture, TexCoord);
};