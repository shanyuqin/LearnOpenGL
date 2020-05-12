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

const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";



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
    
    
    //创建橘色片段着色器
    int fragmentShaderOrange = glad_glCreateShader(GL_FRAGMENT_SHADER);
    glad_glShaderSource(fragmentShaderOrange,1,&fragmentShader1Source,NULL);
    glad_glCompileShader(fragmentShaderOrange);
    
    //创建黄色片段着色器
    int fragmentShaderYellow = glad_glCreateShader(GL_FRAGMENT_SHADER);
    glad_glShaderSource(fragmentShaderYellow,1,&fragmentShader2Source,NULL);
    glad_glCompileShader(fragmentShaderYellow);

    //将着色器连接(Link)为着色器程序
    int shaderProgramOrange = glad_glCreateProgram();
    glad_glAttachShader(shaderProgramOrange,vertexShader);
    glad_glAttachShader(shaderProgramOrange,fragmentShaderOrange);
    glad_glLinkProgram(shaderProgramOrange);
    
    int shaderProgramYellow = glad_glCreateProgram();
    glad_glAttachShader(shaderProgramYellow,vertexShader);
    glad_glAttachShader(shaderProgramYellow,fragmentShaderYellow);
    glad_glLinkProgram(shaderProgramYellow);
    
  
    glad_glDeleteShader(vertexShader);
    glad_glDeleteShader(fragmentShaderOrange);
    glad_glDeleteShader(fragmentShaderYellow);
    

    
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
    unsigned  int VBOs[2] ,VAOs[2];
    glad_glGenVertexArrays(2,VAOs);
    glad_glGenBuffers(2,VBOs);
    
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


 
    
   /**
    索引缓冲对象
    当我们需要画一个矩形的时候其实需要四个顶点，而openGL通过画两个三角形来处理，两个三角形需要6个顶点，而其中有两个顶点重复了两次，当需要画成千个三角形这势必会产生浪费。
    更好的解决方案是只储存不同的顶点，并设定绘制这些顶点的顺序。之后只要指定绘制的顺序就行了
    索引缓冲对象的工作方式正是这样的
    */

/**  画矩形  start
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   // 右上角 0
         0.5f, -0.5f, 0.0f,   // 右下角 1
        -0.5f, -0.5f, 0.0f,   // 左下角 2
        -0.5f,  0.5f, 0.0f    // 左上角 3
    };

    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    
    
    unsigned int VBO, VAO, EBO;
    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    glad_glGenBuffers(1, &EBO);
    glad_glBindVertexArray(VAO);
    
    
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);

画矩形  end */
    
    
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);

        
        glClearColor(0.2f, 0.3f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glad_glUseProgram(shaderProgramOrange);
        glad_glBindVertexArray(VAOs[0]);
        glad_glDrawArrays(GL_TRIANGLES,0,3);
        
        glad_glUseProgram(shaderProgramYellow);
        glad_glBindVertexArray(VAOs[1]);
        glad_glDrawArrays(GL_TRIANGLES,0,3);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glad_glDeleteVertexArrays(2,VAOs);
    glad_glDeleteBuffers(2,VBOs);
    glad_glDeleteProgram(shaderProgramOrange);
    glad_glDeleteProgram(shaderProgramYellow);

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


