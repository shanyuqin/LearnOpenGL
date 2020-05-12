//
//  main.cpp
//  OpenGL01
//
//  Created by ShanYuQin on 2020/5/12.
//  Copyright © 2020 ShanYuQin. All rights reserved.
//
#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//着色器源代码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
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
    
/**  画三角形  start
    
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
    
       // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//     glBindBuffer(GL_ARRAY_BUFFER, 0);

     // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
     // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//     glBindVertexArray(0);


 画三角形  end */
    
   /**
    索引缓冲对象
    当我们需要画一个矩形的时候其实需要四个顶点，而openGL通过画两个三角形来处理，两个三角形需要6个顶点，而其中有两个顶点重复了两次，当需要画成千个三角形这势必会产生浪费。
    更好的解决方案是只储存不同的顶点，并设定绘制这些顶点的顺序。之后只要指定绘制的顺序就行了
    索引缓冲对象的工作方式正是这样的
    */


   float firstTriangle[] = {
       -0.9f, -0.5f, 0.0f,  // left
       -0.0f, -0.5f, 0.0f,  // right
       -0.45f, 0.5f, 0.0f,  // top
   };
   float secondTriangle[] = {
       0.0f, -0.5f, 0.0f,  // left
       0.9f, -0.5f, 0.0f,  // right
       0.45f, 0.5f, 0.0f   // top
   };
    
    
    unsigned int VBOs[2], VAOs[2];
    glad_glGenVertexArrays(2, VAOs);
    glad_glGenBuffers(2, VBOs);
    
    glad_glBindVertexArray(VAOs[0]);
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(firstTriangle),firstTriangle,GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);

    glad_glBindVertexArray(VAOs[1]);
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(secondTriangle),secondTriangle,GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    
    
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);

        
        glClearColor(0.2f, 0.3f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glad_glUseProgram(shaderProgram);
        glad_glBindVertexArray(VAOs[0]);
        glad_glDrawArrays(GL_TRIANGLES,0,3);
        glad_glBindVertexArray(VAOs[1]);
       glad_glDrawArrays(GL_TRIANGLES,0,3);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glad_glDeleteVertexArrays(2,VAOs);
    glad_glDeleteBuffers(2,VBOs);
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


