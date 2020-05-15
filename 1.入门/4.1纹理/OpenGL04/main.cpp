//
//  main.cpp
//  OpenGL04
//
//  Created by ShanYuQin on 2020/5/13.
//  Copyright © 2020 ShanYuQin. All rights reserved.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "警告标识符"



#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <cmath>
#include "shader.h"

//通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了。
//现在只需要在你的程序中包含stb_image.h并编译就可以了。
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<filesystem>
using namespace std;
//using namespace std::__fs::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    //创建一个窗口对象
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout<<"创建 GLFW Window 失败"<<endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout<<"初始化glad失败"<<endl;
        return -1;
    }
    
//使用着色器类创建一个着色器程序shaderProgram
    
    Shader ourShader("shader.vs","shader.fs");
    
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
//    练习2
//    float vertices[] = {
//        // 0.5f    ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
//         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
//        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
//    };
    
//    练习3
//    float vertices[] = {
//        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // 右上
//         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // 右下
//        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // 左下
//        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // 左上
//    };
    
    
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };
    
    unsigned int VBO ,VAO,EBO;
    glad_glGenVertexArrays(1,&VAO);
    glad_glGenBuffers(1,&VBO);
    glad_glGenBuffers(1,&EBO);
    
    glad_glBindVertexArray(VAO);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //    第四个参数实际上就是一个偏移量，因为添加了颜色之后每一个顶点 包括6个属性 XYZ RGB ，对于每一个顶点来说，位置在前，所以偏移量为0，
    //    而颜色属性紧随位置数据之后，所以偏移量是3*sizeof(float)。
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    
    glad_glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glad_glEnableVertexAttribArray(2);

   
    //创建纹理
    unsigned int texture1,texture2;
    glad_glGenTextures(1,&texture1);
    glad_glBindTexture(GL_TEXTURE_2D,texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    //加载并生成纹理
    int width,height,nrChannels; //颜色通道
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    
    if (data) {
        //生成纹理 当前绑定的纹理对象就会被附加上纹理图像
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        //自动生成所有需要的多级渐远纹理。
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        cout<<"创建纹理失败"<<endl;
    }
    //生成纹理后释放图像内存
    stbi_image_free(data);
    
    
    glad_glGenTextures(1,&texture2);
    glad_glBindTexture(GL_TEXTURE_2D,texture2);
    
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        //生成纹理 当前绑定的纹理对象就会被附加上纹理图像
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        //自动生成所有需要的多级渐远纹理。
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        cout<<"创建纹理失败"<<endl;
    }
    //生成纹理后释放图像内存
    stbi_image_free(data);
    
    // 别忘记在设置uniform前先激活着色器
    ourShader.use();
    //后边的0，1对应的是纹理单元，每一个
    glad_glUniform1i(glad_glGetUniformLocation(ourShader.ID, "texture1"), 0);//手动设置
    ourShader.setInt("texture2", 1);//使用着色器类设置
    
    
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);

        
        glad_glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        // 在绑定纹理之前先激活纹理单元
        glad_glActiveTexture(GL_TEXTURE0);
        glad_glBindTexture(GL_TEXTURE_2D, texture1);
        glad_glActiveTexture(GL_TEXTURE1);
        glad_glBindTexture(GL_TEXTURE_2D, texture2);
//        激活着色器程序
        ourShader.use();
        
        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glad_glDeleteVertexArrays(1,&VAO);
    glad_glDeleteBuffers(1,&VBO);
    glad_glDeleteBuffers(1, &EBO);
    

   //正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
    
    
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    cout<<"framebuffer_size_callback函数被调用了"<<endl;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
//    GLFW_KEY_ESCAPE  是 ESC按键的key。
//    glfwGetKey的返回值为GLFW_PRESS代表当前按下了ESC键
//    下边写出按下esc后做什么处理
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
//        cout<<"按下了ESC"<<endl;
}


