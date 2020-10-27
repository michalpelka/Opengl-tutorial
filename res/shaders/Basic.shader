#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * position;
  TexCoord = aTexCoord;
};
#shader fragment
#version 330 core
uniform vec4 u_Color;

layout(location = 0) out vec4 color; 
in vec2 TexCoord;

uniform sampler2D u_Texture;
void main()
{
	//color = u_Color;
	color = texture(u_Texture, TexCoord);
};