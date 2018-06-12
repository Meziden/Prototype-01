#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef VERTEX_STRUCT
#define VERTEX_STRUCT
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexPos;
};
#endif

struct Texture
{
	int number;
	std::string texture_type;
};

class Model
{
public:
	//Constructor(s)
	Model(const std::string& FilePath);
	Model() = delete;
	
	void render();
	
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<GLuint> indices;
	
	unsigned int indicesOffset;

private:
	GLuint VAO,VBO,EBO;
	void MeshKiller(aiMesh* mesh,const aiScene* scene,unsigned int& indexOffset);
	void NodeKiller(aiNode* node,const aiScene* scene);
};
