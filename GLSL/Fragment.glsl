#version 450 core

out vec4 FragColor;

in vec2 TexPos;
in vec3 VecNor;
in vec3 FragPos;

uniform vec3 ViewPos;
uniform vec4 NodeColor;
in vec3 lightPos;

vec3 lightDir = normalize(FragPos - lightPos);

void main()
{
	vec3 ambient = vec3(0.3,0.3,0.3);
	
	vec3 diffuse = vec3(max(dot(-lightDir,VecNor),0.0));
	
	vec3 reflectDir = reflect(lightDir,VecNor);
	vec3 viewDir = normalize(FragPos - ViewPos);
	vec3 specular = vec3(pow(max(dot(-viewDir,reflectDir),0.0),256));
	
	FragColor = vec4((ambient+diffuse+specular),1.0f)*NodeColor;
}
