//
//  main.cpp
//  OpenGL03
//
//  Created by ShanYuQin on 2020/5/12.
//  Copyright © 2020 ShanYuQin. All rights reserved.
//
#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <cmath>

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//着色器源代码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
//    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos,1.0);\n"
//    " vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
//    "in vec4 vertexColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
//    "   FragColor = vertexColor;\n"
    "   FragColor = ourColor;\n"
    "}\0";


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
    
//创建一个着色器程序shaderProgram
    
    //创建顶点着色器
    int vertexShader = glad_glCreateShader(GL_VERTEX_SHADER);
    glad_glShaderSource(vertexShader,1,&vertexShaderSource,nullptr);
    glad_glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glad_glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if (!success) {
        glad_glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<endl;
    }
    
    //创建片段着色器
    int fragmentShader = glad_glCreateShader(GL_FRAGMENT_SHADER);
    glad_glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glad_glCompileShader(fragmentShader);
    glad_glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if (!success) {
        glad_glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<endl;
    }
    
    //将着色器连接(Link)为着色器程序
    int shaderProgram = glad_glCreateProgram();
    glad_glAttachShader(shaderProgram,vertexShader);
    glad_glAttachShader(shaderProgram,fragmentShader);
    glad_glLinkProgram(shaderProgram);
    
    glad_glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if (!success) {
        glad_glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<<infoLog<<endl;
    }
    glad_glDeleteShader(vertexShader);
    glad_glDeleteShader(fragmentShader);
    

    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };
    unsigned int VBO ,VAO;
    glad_glGenVertexArrays(1,&VAO);
    glad_glGenBuffers(1,&VBO);
    glad_glBindVertexArray(VAO);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    

    
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);

        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

//        激活着色器程序
        glad_glUseProgram(shaderProgram);
        
//        更新uniform颜色
//        更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)/2.0f) + 0.5f;
        int vertexColorLocation = glad_glGetUniformLocation(shaderProgram,"ourColor");
        glad_glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        
        
        glad_glBindVertexArray(VAO);
        glad_glDrawArrays(GL_TRIANGLES,0,3);

        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glad_glDeleteVertexArrays(1,&VAO);
    glad_glDeleteBuffers(1,&VBO);

    glad_glDeleteProgram(shaderProgram);

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


