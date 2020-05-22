#include <glad/glad.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include "shader.h"
#include "Camera.h"
#include "model.h"

//通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了。
//现在只需要在你的程序中包含stb_image.h并编译就可以了。
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

using namespace std;

GLFWwindow *initWindow();
bool initSomething(GLFWwindow * window);
unsigned int loadTexture( char const * path);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 10.0f, 75.0f));
bool firstMouse = true;
float lastX =  SCR_WIDTH / 2.0f;
float lastY =  SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

int main()
{
    GLFWwindow *window = initWindow();
    if (window == NULL) {
        return -1;
    }
    
    if (!initSomething(window)) {
        return -1;
    }
    
    //    开启深度测试
    glad_glEnable(GL_DEPTH_TEST);

    camera.MovementSpeed = 40.0f;
    Model rock("objects/rock/rock.obj");
    Model planet("objects/planet/planet.obj");
    //小行星
    Shader asteroidShader("asteroid.vs","asteroid.fs");
    //大行星
    Shader planetShader("planet.vs","planet.fs");


    unsigned int amount = 10000;
    glm::mat4 * modelMat = new glm::mat4[amount];
    srand(glfwGetTime());//初始化随机种子
    float radius = 70.0;
    float offset = 5.0f;
    
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0);
        
        //这个计算需要好好理解
        float angle = (float)i / float(amount) * 360.0f;
        float displacement = (rand() % (int)(2 * offset *100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. 缩放：在 0.05 和 0.25f 之间缩放
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4. 添加到矩阵的数组中
        modelMat[i] = model;
    }
    
    unsigned int buffer;
    glad_glGenBuffers(1, &buffer);
    glad_glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount,&modelMat[0],GL_STATIC_DRAW);
    for (unsigned int i = 0; i < rock.meshes.size(); i++) {
        unsigned int VAO = rock.meshes[i].VAO;
        glad_glBindVertexArray(VAO);
        GLsizei vec4Size = sizeof(glm::vec4);
        glad_glEnableVertexAttribArray(3);
        glad_glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glad_glEnableVertexAttribArray(4);
        glad_glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glad_glEnableVertexAttribArray(5);
        glad_glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glad_glEnableVertexAttribArray(6);
        glad_glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
        
        glad_glVertexAttribDivisor(3, 1);
        glad_glVertexAttribDivisor(4, 1);
        glad_glVertexAttribDivisor(5, 1);
        glad_glVertexAttribDivisor(6, 1);
        glad_glBindVertexArray(0);
    }
        
//    渲染循环
    while (!glfwWindowShouldClose(window)) {

        //deltaTime 用来保证渲染速度
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //      键盘输入
        processInput(window);
        
        //渲染逻辑 start ——————————————————————————————————————————
        glad_glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        asteroidShader.use();
        asteroidShader.setMat4("projection", projection);
        asteroidShader.setMat4("view", view);

        planetShader.use();
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        planetShader.setMat4("model", model);
        planet.Draw(planetShader);
        
        asteroidShader.use();
        asteroidShader.setInt("texture_diffuse1", 0);
        glad_glActiveTexture(GL_TEXTURE0);
        glad_glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
        
        for (unsigned int i = 0; i < rock.meshes.size(); i++)
        {
            glad_glBindVertexArray(rock.meshes[i].VAO);
            glad_glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            glad_glBindVertexArray(0);
            
        }
        
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

    return true;
}


unsigned int loadTexture( char const * path)
{
    unsigned int textureID;
    glad_glGenTextures(1 ,&textureID);
    
    int width,height,nrComponents;
    unsigned char * data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glad_glBindTexture(GL_TEXTURE_2D,textureID);
        glad_glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glad_glGenerateMipmap(GL_TEXTURE_2D);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glad_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        
    }else {
        cout<<"创建纹理失败"<<endl;
    }
    
    stbi_image_free(data);
    return textureID;;
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
