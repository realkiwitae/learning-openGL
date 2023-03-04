#include "Model.h"
#include <iostream>

Model::Model()
{

}

Model::~Model()
{

}

void Model::load(const std::string filename){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("./models/"+filename+"/model.obj",aiProcess_Triangulate 
                                                | aiProcess_FlipUVs 
                                                | aiProcess_GenSmoothNormals
                                                | aiProcess_JoinIdenticalVertices);
    if(!scene){
        std::cout << "Model " << filename <<  " failed to load " << importer.GetErrorString() << std::endl;
        return;
    }
    int idx = std::string(filename).rfind("\\");
    name = std::string(filename).substr(idx+1);

    loadNode(scene->mRootNode, scene);
    loadMaterials(scene);
}

void Model::loadNode(aiNode* node, const aiScene* scene){
    for(size_t i = 0; i < node->mNumMeshes;i++){
        loadMesh(scene->mMeshes[node->mMeshes[i]],scene);
    }
    for(size_t i = 0; i < node->mNumChildren;i++){
        loadNode(node->mChildren[i],scene);
    }
}
void Model::loadMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    for(size_t i = 0; i < mesh->mNumVertices;i++){
        vertices.insert(vertices.end(),{mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z});
        if(mesh->mTextureCoords[0]){
            vertices.insert(vertices.end(),{mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
        }else{
            vertices.insert(vertices.end(),{0.f,0.f});           
        }
        vertices.insert(vertices.end(),{-mesh->mNormals[i].x,-mesh->mNormals[i].y,-mesh->mNormals[i].z});
    }
    for(size_t i = 0; i < mesh->mNumFaces;i++){
        aiFace face = mesh->mFaces[i];
        for(size_t j=0;j < face.mNumIndices;j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* newMesh = new Mesh();
    newMesh->create(&vertices[0],&indices[0],vertices.size(),indices.size());
    meshList.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
}
void Model::loadMaterials(const aiScene* scene){
    textureList.resize(scene->mNumMaterials);
    
    for(size_t i = 0; i < scene->mNumMaterials;i++){
        aiMaterial* material = scene->mMaterials[i];
        
        textureList[i] = nullptr;

        if(material->GetTextureCount(aiTextureType_DIFFUSE)){
            aiString path;
            if(material->GetTexture(aiTextureType_DIFFUSE,0,&path) == AI_SUCCESS){
                int idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx+1);

                std::string texPath = "./textures/"+name +"/"+ filename;
                textureList[i] = new Texture(texPath);

                if(!textureList[i]->load()){
                    std::cout << "Failed to load texture at " << texPath << std::endl; 
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
            }
        }

        if(!textureList[i]){
            textureList[i] = new Texture("./textures/plain.png");
            textureList[i]->loadAlpha();
        }

    }
}

void Model::render(){
    for(size_t i = 0; i < meshList.size();i++){
        unsigned int matidx = meshToTex[i];
        if(matidx < textureList.size() && textureList[matidx]){
            textureList[matidx]->use();
        }
        meshList[i]->render();
    }
}

void Model::clear(){
    for(size_t i = 0; i < meshList.size();i++){
        if(meshList[i]){
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }
    for(size_t i = 0; i < textureList.size();i++){
        if(textureList[i]){
            delete textureList[i];
            textureList[i] = nullptr;
        }
    }
}