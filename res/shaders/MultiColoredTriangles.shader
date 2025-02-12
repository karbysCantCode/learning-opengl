#shader vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 fragColor;

uniform mat4 u_MVP;

void main()
{
	gl_Position =  u_MVP * vec4(position,1.0);
	fragColor = color;
};



#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 fragColor;

void main()
{
	color = fragColor;
};

