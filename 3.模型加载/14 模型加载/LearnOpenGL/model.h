//
//  model.h
//  LearnOpenGL
//
//  Created by ShanYuQin on 2020/5/17.
//  Copyright © 2020 ShanYuQin. All rights reserved.
//

#ifndef model_h
#define model_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//现在只需要在你的程序中包含stb_image.h并编译就可以了。
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "mesh.h"
#include "shader.h"
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model {
public:
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    void Draw(Shader shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    
private:
    void loadModel(string const &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout<<"ASSIMP读取文件失败："<<importer.GetErrorString()<<endl;
            return;
        }
        directory = path.substr(0,path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode *node, const aiScene *scene)
    {
        //处理当前节点中的所有mesh
        for (unsigned int i = 0; i<node->mNumMeshes; i++) {
            //aiNode中的mMeshes 存储的是一个索引，通过这个索引在scene的mMeshes总才找到最终的Meshe
            aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        //递归处理子节点中的mesh索引
        for (unsigned int i = 0; i<node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }
    //将assimp中取得的mesh，转换为我们自己的Mesh对象
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        //遍历网格的所有顶点
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;
            //vertex.Position
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            //vertex.Normal
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            //Assimp允许一个模型在一个顶点上有最多8个不同的纹理坐标,我们不会用那么多，只关心第一组纹理坐标
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            //后边两个在文章中并没有做解释
            // tangent 切线
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent 双切线
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            vertices.push_back(vertex);
        }
        //对一个网格都有一个面(face)数组，一个面就代表一个图元(图元的概念在你好三角形中做了解释)
        //就是我们之前一直传入的GL_TRIANGLES，你希望这些数据所表示的渲染类型
        for (unsigned int i = 0; i<mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                 indices.push_back(face.mIndices[j]);
            }
        }
        //处理材料，每一个mesh包含着一个材料的索引mMaterialIndex，材料真正存储的位置在scene中
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        //接下来加载网格的漫反射和/或镜面光贴图。一个材质对象的内部对每种纹理类型都存储了一个纹理位置数组
        //这里注意我们在mesh中对纹理类型的定义。
        //漫反射贴图
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //镜面光贴图
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
//        vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//
//        vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        return Mesh(vertices, indices, textures);
    }
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            //GetTexture函数可以获取每个纹理的文件名称
            mat->GetTexture(type, i, &str);
            //判断纹理是否已经load，如果load过，直接进行下一次迭代
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    
    unsigned int textureID;
    glad_glGenTextures(1,&textureID);
    
    int width,height,nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glad_glBindTexture(GL_TEXTURE_2D, textureID);
        glad_glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glad_glGenerateMipmap(GL_TEXTURE_2D);
        
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }else {
        cout << "纹理加载失败，路径: " << path << endl;
    }
    stbi_image_free(data);
    return textureID;
}







#endif /* model_h */
