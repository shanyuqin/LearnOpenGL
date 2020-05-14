
#include <glad/glad.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f;


// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f,  -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间


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
//    隐藏光标，并捕捉(Capture)它
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    鼠标一移动mouse_callback函数就会被调用
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout<<"初始化glad失败"<<endl;
        return -1;
    }
//    启用深度测试
    glad_glEnable(GL_DEPTH_TEST);
    
//使用着色器类创建一个着色器程序shaderProgram
    
    Shader ourShader("/Users/shanyuqin/Desktop/LearnOpenGL/OpenGL07——摄像机/OpenGL07——摄像机/shader.vs","/Users/shanyuqin/Desktop/LearnOpenGL/OpenGL07——摄像机/OpenGL07——摄像机/shader.fs");
    

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    //创建更多的立方体，这里设置了10个cube在 世界空间的位置
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    unsigned int VBO ,VAO;
    glad_glGenVertexArrays(1,&VAO);
    glad_glGenBuffers(1,&VBO);
    
    glad_glBindVertexArray(VAO);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
//    最后一个参数实际上就是一个偏移量，因为添加了颜色之后每一个顶点 包括6个属性 XYZ RGB ，对于每一个顶点来说，位置在前，所以偏移量为0，
//    而颜色属性紧随位置数据之后，所以偏移量是3*sizeof(float)。
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glad_glEnableVertexAttribArray(1);
       
    
    unsigned int texture1,texture2;
    //创建纹理1
    glad_glGenTextures(1,&texture1);
    glad_glBindTexture(GL_TEXTURE_2D,texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    //加载并生成纹理
    int width,height,nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("/Users/shanyuqin/Desktop/LearnOpenGL/OpenGL07——摄像机/OpenGL07——摄像机/container.jpg", &width, &height, &nrChannels, 0);
    
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
    
    //创建纹理2
    glad_glGenTextures(1,&texture2);
    glad_glBindTexture(GL_TEXTURE_2D,texture2);
    
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    data = stbi_load("/Users/shanyuqin/Desktop/LearnOpenGL/OpenGL07——摄像机/OpenGL07——摄像机/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        cout<<"创建纹理失败"<<endl;
    }
    stbi_image_free(data);
    
    // 别忘记在设置uniform前先激活着色器,因为纹理
    ourShader.use();
    ourShader.setInt("texture1", 0);//使用着色器类设置
    ourShader.setInt("texture2", 1);//使用着色器类设置
    
    glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)SCR_WIDTH/(float)SCR_WIDTH, 0.1f , 100.f);
    ourShader.setMatrix4fv("projection", projection);
    
    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        
        glad_glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 在绑定纹理之前先激活纹理单元
        glad_glActiveTexture(GL_TEXTURE0);
        glad_glBindTexture(GL_TEXTURE_2D, texture1);
        glad_glActiveTexture(GL_TEXTURE1);
        glad_glBindTexture(GL_TEXTURE_2D, texture2);
        
        ourShader.setFloat("mixValue", mixValue);
        
        
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::translate(view, glm::vec3(1.0f, 0.0f, -3.0f));

                
//        激活着色器程序
        ourShader.use();
        
        
//         float radius = 10.0f;
//         float camX = sin(glfwGetTime()) * radius;
//         float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view = glm::mat4(1.0f);
         view = glm::lookAt(cameraPos, cameraFront+cameraPos, cameraUp);
         
        
        
        ourShader.setMatrix4fv("view", view);

        
        glad_glBindVertexArray(VAO);
        
        
        
        for (int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i+1);
             model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            
            ourShader.setMatrix4fv("model", model);
            glad_glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glad_glDeleteVertexArrays(1,&VAO);
    glad_glDeleteBuffers(1,&VBO);
    
    

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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
//        cout<<"按下了ESC"<<endl;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue += 0.01f;
        if (mixValue > 1.0f) {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
           mixValue -= 0.01f;
           if (mixValue <= 0.0f) {
               mixValue = 0.0f;
           }
    }
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    
    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
