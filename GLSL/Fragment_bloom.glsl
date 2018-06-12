#version 450 core

out vec4 FragColor;

in vec2 TexPos;
uniform sampler2D fbotexture;

const float offset = 0.35 / 100.0; 

void main()
{
	vec2 offsets[25] = vec2[]
	(
		vec2(-2*offset,2*offset),	vec2(-offset,2*offset),	vec2(0.0f,2*offset),	vec2(offset,2*offset),	vec2(2*offset,2*offset),
		vec2(-2*offset,offset),		vec2(-offset,offset),	vec2(0.0f,offset),		vec2(offset,offset),	vec2(2*offset,offset),
		vec2(-2*offset,0.0f),		vec2(-offset,0.0f),		vec2(0.0f,0.0f),		vec2(offset,0.0f),		vec2(2*offset,0.0f),
		vec2(-2*offset,-offset),	vec2(-offset,-offset),	vec2(0.0f,-offset),		vec2(offset,-offset),	vec2(2*offset,-offset),
		vec2(-2*offset,-2*offset),	vec2(-offset,-2*offset),vec2(0.0f,-2*offset),	vec2(offset,-2*offset),	vec2(2*offset,-2*offset)
	);
	
	float kernel[25] = float[]
	(
		1.0/256,4.0/256,6.0/256,4.0/256,1.0/256,
		4.0/256,16.0/256,24.0/256,16.0/256,4.0/256,
		6.0/256,24.0/256,36.0/256,24.0/256,6.0/256,
		4.0/256,16.0/256,24.0/256,16.0/256,4.0/256,
		1.0/256,4.0/256,6.0/256,4.0/256,1.0/256
	);
	
	vec3 bloomTex[25];

	for(int i = 0; i < 25; i++)
	{
		bloomTex[i] = vec3(texture(fbotexture, TexPos.st + offsets[i]));
		bloomTex[i] = max((bloomTex[i] - 0.65) * 4.0 ,vec3(0.0,0.0,0.0));
	}
	
	vec3 sampleTex;
	sampleTex = vec3(texture(fbotexture, TexPos.st));
	
	vec3 col = vec3(0.0);
	for(int i = 0; i < 25; i++)
		col += sampleTex[i] * kernel[i];
	
	//col = sampleTex + col;
		
	FragColor = vec4(col, 1.0);
}
