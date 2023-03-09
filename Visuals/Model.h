#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void load(const std::string& fileName);
	void render();
	void clear();

	~Model();

private:

	void loadNode(aiNode *node, const aiScene *scene);
	void loadMesh(aiMesh *mesh, const aiScene *scene);
	void loadMaterials(const aiScene *scene);
	std::string name;
	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
};

