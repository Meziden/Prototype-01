#include "Model.h"

Model::Model(const std::string& FilePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(FilePath,aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_OptimizeMeshes);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		std::cout << "Model::ASSIMP::" << importer.GetErrorString() << std::endl;
	
	indicesOffset=0;
	
	NodeKiller(scene -> mRootNode,scene);
	
	glCreateVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glCreateBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glNamedBufferStorage(VBO,sizeof(Vertex) * vertices.size(),&vertices[0],0);
	glCreateBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glNamedBufferStorage(EBO,sizeof(unsigned int) * indices.size(),&indices[0],0);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Normal));
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,TexPos));
}

void Model::MeshKiller(aiMesh* mesh,const aiScene* scene,unsigned int& indicesOffset)
{
	for(unsigned int i = 0;i < mesh -> mNumVertices;i++)
	{
		Vertex vertex_tmp;
		
		glm::vec3 vector;
		vector.x = mesh -> mVertices[i].x;
		vector.y = mesh -> mVertices[i].y;
		vector.z = mesh -> mVertices[i].z;
		vertex_tmp.Position = vector;
		
		vector.x = mesh -> mNormals[i].x;
		vector.y = mesh -> mNormals[i].y;
		vector.z = mesh -> mNormals[i].z;
		vertex_tmp.Normal = vector;
		
		vertex_tmp.TexPos = glm::vec2(0.0f, 0.0f);
		
		vertices.push_back(vertex_tmp);
	}
	
	for(unsigned int i = 0; i < mesh -> mNumFaces; i++)
	{
		aiFace face = mesh -> mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j] + indicesOffset);
	}
	indicesOffset = indices.back() - indices.front() + 1;
}

void Model::NodeKiller(aiNode* node,const aiScene* scene)
{
	for(unsigned int i = 0;i< node-> mNumMeshes;i++)
	{
		aiMesh* mesh = scene -> mMeshes[node -> mMeshes[i]];
		MeshKiller(mesh,scene,indicesOffset);
	}
	
	for(unsigned int i = 0;i< node-> mNumChildren;i++)
	{
		NodeKiller(node -> mChildren[i],scene);
	}
}

void Model::render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}
