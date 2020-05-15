#include <glad/glad.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include "shader.h"
#include "Camera.h"

//通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了。
//现在只需要在你的程序中包含stb_image.h并编译就可以了。
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<filesystem>
using namespace std;
GLFWwindow *initWindow();
bool initSomething(GLFWwindow * window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f;


// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f,  -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间


//lighiting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    GLFWwindow *window = initWindow();
    if (window == NULL) {
        return -1;
    }
    
    if (!initSomething(window)) {
        return -1;
    }
    
    Shader lightingShader("colors.vs","colors.fs");
    Shader lampShader("lamp.vs","lamp.fs");
    
    float vertices[] = {
           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,

           -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,

           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f, -0.5f,

           -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
       };
    
    
    unsigned int VBO, cubeVAO , lightVAO;
    glad_glGenVertexArrays(1,&cubeVAO);
    glad_glGenBuffers(1,&VBO);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glad_glBindVertexArray(cubeVAO);
    
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glGenVertexArrays(1,&lightVAO);
    glad_glBindVertexArray(lightVAO);
//  只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
//   设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);

    
//    渲染循环
    while (!glfwWindowShouldClose(window)) {

        //deltaTime 用来保证渲染速度
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //      键盘输入
        processInput(window);
        
        glad_glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        //渲染逻辑 start ——————————————————————————————————————————
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f,0.5f,0.31f);
        lightingShader.setVec3("lightColor", 1.0f,1.0f,1.0f);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMatrix4fv("projection", projection);
        lightingShader.setMatrix4fv("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMatrix4fv("model", model);
        
        //渲染cube
        glad_glBindVertexArray(cubeVAO);
        glad_glDrawArrays(GL_TRIANGLES,0,36);
        
        lampShader.use();
        lampShader.setMatrix4fv("projection", projection);
        lampShader.setMatrix4fv("view", view);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMatrix4fv("model", model);
        
        
        glad_glBindVertexArray(lightVAO);
        glad_glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //渲染逻辑 end ——————————————————————————————————————————
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}


GLFWwindow *initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
       
    GLFWwindow *window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL",NULL,NULL);
    if (window == NULL) {
        cout<<"创建GLFW 窗口失败"<<endl;
    }
    
    return window;
}

bool initSomething(GLFWwindow * window) {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
//    进入窗口隐藏鼠标，捕获鼠标移动
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
//    使用gl的API之前要先加载GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout<<"failed to initialize glad"<<endl;
        return false;
    }
//    开启深度测试
    glad_glEnable(GL_DEPTH_TEST);
    return true;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glad_glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
