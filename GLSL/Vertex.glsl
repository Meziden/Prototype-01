#version 450 core

layout (location = 0) in vec3 VecPos;
layout (location = 1) in vec3 VecNorV;
layout (location = 2) in vec2 TexPosV;

out vec2 TexPos;
out vec3 VecNor;
out vec3 FragPos;
out vec3 lightPos;
uniform mat4 trans_mod;
uniform mat4 trans_view;
uniform mat4 trans_proj;
uniform vec3 lightPosV;

void main()
{
	VecNor = mat3(transpose(inverse(trans_mod))) * VecNorV;
	TexPos = vec2(TexPosV.x,1.0 - TexPosV.y);
	gl_Position = trans_proj * trans_view * trans_mod * vec4(VecPos,1.0);
	FragPos = vec3(trans_mod * vec4(VecPos,1.0));
	lightPos = lightPosV;
}
