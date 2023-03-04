#ifndef MODEL_H
#define MODEL_H

#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../utils/Mesh.h"
#include "../textures/Texture.h"

class Model
{
public:
    Model();
    ~Model();
    void load(const std::string filename);
    void render();
    void clear();
private:

    void loadNode(aiNode* node, const aiScene* scene);
    void loadMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterials(const aiScene* scene);
    std::string name;
    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTex;
};

#endif