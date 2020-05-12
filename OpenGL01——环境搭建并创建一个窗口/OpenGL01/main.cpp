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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    //初始化glfw
    glfwInit();
    //设置openGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
    //明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //如果使用的Mac OS系统需要添加下边代码，这些配置才能起作用
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    //创建一个窗口对象
    GLFWwindow *window = glfwCreateWindow(350, 667, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout<<"创建 GLFW Window 失败"<<endl;
        glfwTerminate();
        return -1;
    }
    //通知GLFW将我们的窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //给窗口注册了一个回调函数，他会在每次窗口大小被调整的时候被调用
    //当窗口被第一次显示的时候 这个注册的回调也会被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    
    //初始化GLAD ，实际上就是加载所有openGL的函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout<<"初始化glad失败"<<endl;
        return -1;
    }
    
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);

        
        glClearColor(0.2f, 0.3f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
        //它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        glfwSwapBuffers(window);
        //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
        glfwPollEvents();
    }

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


