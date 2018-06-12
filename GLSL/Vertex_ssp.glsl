#version 450 core

layout (location = 0) in vec3 VecPosV;
layout (location = 1) in vec2 TexPosV;

out vec2 TexPos;

void main()
{
	gl_Position = vec4(VecPosV,1.0);
	TexPos = TexPosV;
}
