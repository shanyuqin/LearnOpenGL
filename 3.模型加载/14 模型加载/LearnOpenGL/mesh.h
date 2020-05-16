//
//  mesh.h
//  LearnOpenGL
//
//  Created by ShanYuQin on 2020/5/17.
//  Copyright © 2020 ShanYuQin. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

using namespace std;
//顶点结构体
struct Vertex {
    //位置坐标
    glm::vec3 Position;
    //法向量
    glm::vec3 Normal;
    //纹理坐标
    glm::vec2 TexCoords;
};

struct Texture {
    //纹理ID （textureID）glad_glGenTextures(1 ,&textureID);
    unsigned int id;
    //纹理类型
    string type;
    //纹理路径
    string path;
};

class Mesh {
public:
    //
    vector<Vertex>          vertices;
    vector<unsigned int>    indices;
    vector<Texture>         textures;
    unsigned int VAO;
    
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }
    
    void Draw(Shader shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        
        for (unsigned int i = 0; i < textures.size(); i++) {
            glad_glActiveTexture(GL_TEXTURE0+i);
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = to_string(specularNr++);
            else if(name == "texture_normal")
                number = to_string(normalNr++);
            else if(name == "texture_height")
                number = to_string(heightNr++);
            
            shader.setInt(name+number, i);
            glad_glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        
        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
        glad_glBindVertexArray(0);

        //是在配置后将所有设置都恢复为默认值。
        glad_glActiveTexture(GL_TEXTURE0);

        
    }
private:
    unsigned int VBO,EBO;
    void setupMesh()
    {
        glad_glGenVertexArrays(1, &VAO);
        glad_glGenBuffers(1, &VBO);
        glad_glGenBuffers(1, &EBO);
        
        glad_glBindVertexArray(VAO);
        
        glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glad_glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        // set the vertex attribute pointers
        // vertex Positions
        glad_glEnableVertexAttribArray(0);
        glad_glVertexAttribPointer(0 ,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glad_glEnableVertexAttribArray(0);
        glad_glVertexAttribPointer(0 ,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glad_glEnableVertexAttribArray(0);
        glad_glVertexAttribPointer(0 ,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        
    }
};





#endif /* mesh_h */
