# LearnOpenGL
[LearnOpenGL CN](https://learnopengl-cn.github.io)的学习笔记

# 入门
## 1.环境搭建并创建一个窗口
下边的逻辑保证我们的程序在我们主动关闭之前，能够不断的绘制图像，接受用户输入。这个while循环能在我们让GLFW退出之前一直保持运行。
```
while(!glfwWindowShouldClose(window))
{
    glfwSwapBuffers(window);
    glfwPollEvents();    
}
```
glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
## 2.画一个橙色三角形 
[参考](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/)
>顶点数组对象：Vertex Array Object，VAO
>顶点缓冲对象：Vertex Buffer Object，VBO
>索引缓冲对象：Element Buffer Object，EBO

#### VBO
顶点数据（float vertices[]={...};）作为输入发送给图形渲染管线的第一个处理阶段：顶点着色器。顶点着色器源文件中的layout (location = 0) in vec3 aPos;就是负责接收这个顶点数据的。
顶点着色器会在GPU上创建内存用于储存我们的顶点数据。而VBO就是用来管理这个内存的。
```
//生成VBO，这个缓冲有一个独一无二的ID，这里将ID设为1
unsigned int VBO;
glGenBuffers(1, &VBO); 
//绑定缓冲类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
glBindBuffer(GL_ARRAY_BUFFER, VBO); 
//把之前定义的顶点数据vertices复制到缓冲的内存中：
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```
[链接顶点属性](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/)
```
//第一个参数代表顶点着色器中layout (location = 0) in vec3 aPos;中的location，代表一个位置
//第二个参数代表这个aPos参数占多少位 XYZ  占了三位。
//第五个参数叫做步长(Stride)，其实就是一个顶点数据占用的内存大小
//最后一个个参数实际上就是一个偏移量，因为添加了颜色之后每一个顶点 包括6个属性 XYZ RGB ，对于每一个顶点来说，位置在前，所以偏移量为0，
//而颜色属性紧随位置数据之后，所以偏移量是3*sizeof(float)。
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//顶点属性默认是禁用的，需要手动启用顶点数据
glEnableVertexAttribArray(0);
```
举个例子
```
main.cpp

float vertices[] = {
     //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
 };
 
 glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
 glad_glEnableVertexAttribArray(0);
 
 glad_glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
 glad_glEnableVertexAttribArray(1);
 
 glad_glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
 glad_glEnableVertexAttribArray(2);
 
 
 shader.vs
 
 layout (location = 0) in vec3 aPos;
 layout (location = 1) in vec3 aColor;
 layout (location = 2) in vec2 aTexCoord;
```

上边示例中每个顶点包含三块数据，所以顶点着色器有三个参数，相对位置为0，1，2.
链接顶点属性的时候先后对三个属性进行了链接。
aPos,位置为0，需要3个float来表示，步长为8*sizeof(float)，每次都是从位置0的内存位置来读取数据。
aColor,位置为1，需要3个float来表示，步长为8*sizeof(float)，每次都是从位置(3*sizeof(float))的内存位置来读取数据。
aTexCoord,位置为2，需要2个float来表示，步长为8*sizeof(float)，每次都是从位置(6*sizeof(float))的内存位置来读取数据。


#### VAO
它其实是一个数组里边存储多个VBO。要想使用VAO，要做的只是使用glBindVertexArray绑定VAO。

## 3.1着色器uniform的使用
主要介绍uniform的使用。
## 3.2为着色器添加更多属性
为顶点着色器添加了一个颜色属性。每一个顶点就包括了六个数据分别是 XYZRGB
定义顶点着色器的时候看到的layout （location = 0） in vec3 XXX;这里的location其实就是这个顶点向量的一个相对位置。
具体可以看代码中顶点着色器源代码的注释。
## 3.3定义自己的着色器类
通过字符串编写着色器的源代码，然后在进行编译和管理，比较麻烦，这里写了一个类来从硬盘中读取着色器，然后编译并链接他们，以及进行错误检查。
但是对于ifstream的文件读取这里只能使用绝对路径么？这里是一个疑问
补充：
Product > Scheme > Edit Scheme > Run> Options Use custom working directory打勾，然后设置你放文知件的位置，这样就不需要添加绝对路道径了
但是也看到通过自己写相关类进行处理的方式但是我没有编译成功，有看明白的希望能联系我[连接](https://www.jianshu.com/p/749a781fef94)


## 4.纹理
### 纹理单元
当我们只使用一个模板的纹理时，我们在片段着色器源文件中声明了uniform 的采样器(sampler2D)变量，但是我们并没有使用glUniform对其进行赋值。原因是什么？
一个纹理的位置值通常成为一个纹理单元。一个纹理的默认纹理单元是0，它是默认已经被激活的。所以只使用一个纹理的时候不需要进行赋值和激活。

在后来的示例中，我们在同一个位置使用了两张图片，添加了两个纹理。这时候我们可以通过`glUniform1i`，给纹理采样器分配一个位置值，这样的话我们能够在一个片段着色器中设置多个纹理。
如果此时还不进行赋值，那么只会显示最后一次绑定的纹理，比如示例中 ，我们在渲染循环之前先后进行了，箱子和笑脸的绑定，并没有对纹理单元进行赋值，也没有在渲染循环内部进行纹理单元的激活。

正确的做法是在渲染循环之前，通过`glUniform1i`，对两个纹理单元进行赋值。
`glad_glUniform1i(glad_glGetUniformLocation(ourShader.ID, "texture1"), 0);`
`glad_glUniform1i(glad_glGetUniformLocation(ourShader.ID, "texture2"), 1);`
后边的0和1就是纹理单元的位置值，OpenGL保证至少有16个纹理单元供你使用。
然后在渲染循环中进行纹理单元的激活和绑定
```
glad_glActiveTexture(GL_TEXTURE0);
glad_glBindTexture(GL_TEXTURE_2D, texture1);
glad_glActiveTexture(GL_TEXTURE1);
glad_glBindTexture(GL_TEXTURE_2D, texture2);
```

对于纹理渲染使用的图片，要想达到官网上的例子效果，那么图片必须是无alpha通道。
有alpha通道的情况是这样的
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/4-0.png" width="50%">

## 5.矩阵/变换/向量/GLM
主要是一些线性代数的内容，矩阵和向量。然后还有GLM库的使用。
安装glm
`brew install glm`

我们需要的GLM的大多数功能都可以从下面这3个头文件中找到：
```
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```
涉及到变换的内容，对于变换的前后顺序不同导致的结果，还需要进行探索。
比如先移动再旋转，和先旋转再移动，最终的导致结果的原因是什么。
```
//GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0）。
//下边代码是生成了一个单位矩阵
glm::mat4 trans = glm::mat4(1.0f);

//平移将箱子放到右下角 然后 随着时间转动
trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
```
这里需要记住向量相乘的两个概念点乘和叉乘
点乘v¯⋅k¯ 计算的是两个向量夹角的余弦值。（v,k都是单位向量）
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/5-0.png" width="50%">
叉乘v¯ x k¯ 只在3D空间中有定义，它需要两个不平行向量作为输入，生成一个正交于两个输入向量的第三个向量
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/5-1.png" width="50%">

线性代数的内容可以参考[【官方双语/合集】线性代数的本质 - ](https://www.bilibili.com/video/BV1ys411472E)
## 6.坐标系统
理解局部空间，世界空间，观察空间，裁剪空间，屏幕空间
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/6-0.png" width="50%">
### 局部空间
物指体所在的坐标空间，即对象最开始所在的地方。比如我们创建一个房子，那么我们在单独创建这个房子的时候为它定义了一个坐标空间。房子旁边需要有一个花园，那么我们单独创建这个花园的时候又定义了一个坐标空间，房子的坐标空间和花园的坐标空间是完全不相关的。是属于它们二者自身的坐标空间。也就是局部坐标空间。
### 世界空间
最终我们肯定是要将房子和花园，都放进我们的一个虚拟的世界中，那么这时候就需要一个世界空间，那么如何放置房子和花园？不做任何变换，那么他们都会堆在世界空间的原点(0,0,0)的位置。我们希望房子和 花园是挨着的或者中间留有一些缝隙，并且二者要在同一个水平面上。这时二者原来在局部空间下每一个片段的坐标，都会有一个变换。变换之后就是在这个世界空间的坐标了。这个变换就是通过模型变换（model）来实现的。
### 观察空间
如果以人的眼睛为原点，去观察这个房子和花园，那么这个房子和花园的坐标必然会因为你处于的位置不同，而有所变化。比如你在房子的顶部向下观看，那么房子和花园在观察空间的Y坐标肯定是小于0的。当你在房子的底部向上看，Y坐标肯定都是大于0的。这个变换是通过一个观察矩阵来处理的。
处理的结果，就是将我们观察的位置作为一个原点，通过下边的逻辑构造一个坐标空间，这就是观察空间
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/6-1.png" width="50%">
#### look at
GLM已经提供了这个。我们要做的只是定义一个摄像机位置，一个目标位置和一个表示世界空间中的上向量的向量(0,1,0)。接着GLM就会创建一个LookAt矩阵，我们可以把它当作我们的观察矩阵：
```
glm::mat4 view;
view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), //摄像机位置
           glm::vec3(0.0f, 0.0f, 0.0f), //目标位置
           glm::vec3(0.0f, 1.0f, 0.0f));//世界空间中的上向量的向量
```
### 裁剪空间
裁剪空间实际上就是通过 投影矩阵 做了一下处理，给了一个透视的效果。


## 7.FPS摄像机
关于[自由移动](https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/)通过下边的方式生成了一个lookAt矩阵
```
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
//生成lookat
view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
```
原文中有如下解释
>我们首先将摄像机位置设置为之前定义的cameraPos。方向是当前的位置加上我们刚刚定义的方向向量。这样能保证无论我们怎么移动，摄像机都会注视着目标方向。
而在它上边说生成lookAt的时候需要的三个矩阵如下
>glm::LookAt函数需要一个位置、目标和上向量。它会创建一个和在上一节使用的一样的观察矩阵

当我们按WASD改变摄像机位置的时候，为了保证无论怎么移动摄像机都会注释目标方向，那么这个生成lookAt的第二个参数目标向量 就必须也是要随着摄像机位置来变化的。可以想象一下CS或者CF游戏按着WASD进行移动时候的场景，你的准星始终和你的眼睛保持了一个固定的距离。
这里通过cameraPos + cameraFront就是来保证了这一点。初始的时候 相加结果为`(0.0f, 0.0f, 2.0f)`，那么也就是说，无论摄像机位置如何移动，我的观察目标始终在摄像机位置关于Z轴向前移动1个单位。（摄像机是003，观察目标是002，所以你在003的位置观察002，就是摄像机向Z轴负方向移动了1个单位）。

### 视角移动
想想一下CS/CF添加了鼠标的操作。其实原理就是通过鼠标的操作改变了前边的`cameraFront`向量。

### 欧拉角
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/7-0.png" width="50%">
俯仰角是描述我们如何往上或往下看的角，即围绕X轴转动
偏航角表示我们往左和往右看的程度，即围绕Y轴转动。
滚转角代表我们如何翻滚摄像机，即围绕Z轴转动。（想象下拍抖音，旋转手机屏幕，呈现人物头部由上到下的运镜效果）。
每个欧拉角都有一个值来表示，把三个角结合起来我们就能够计算3D空间中任何的旋转向量了。


# 光照
## 8.颜色 
#### 对于你能看到的颜色的理解。
我们在现实生活中看到某一物体的颜色并不是这个物体真正拥有的颜色，而是它所反射的(Reflected)颜色。换句话说，那些不能被物体所吸收(Absorb)的颜色（被拒绝的颜色）就是我们能够感知到的物体的颜色。

如果一个光源是白色的光源，那么你看到的物体颜色，实际上就是它本身的颜色。如果一个光源是其他颜色那你看到的物体颜色，就会根据光源颜色而有所变化。
这个结果该如何计算？我们定义一个颜色向量。
>glm::vec3 coral(1.0f, 0.5f, 0.31f);珊瑚红
光源如果是白色
>glm::vec3 coral(1.0f, 1.0f, 1.0f)
将这两个颜色向量作分量相乘，结果就是你看到的颜色。
换一个光源
>glm::vec3 lightColor(0.0f, 1.0f, 0.0f);绿色光源
>glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
结果你看到的其实是一个深绿色的颜色。
>glm::vec3 result = lightColor * toyColor; // = (0.0f, 0.5f, 0.0f);

## 9.基础光照
对于颜色的计算是基于一个光照模型，这些光照模型都是基于我们对光的物理特性的理解。
这里主要讲了冯氏光照模型(Phong Lighting Model)。
冯氏光照模型的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。
环境光照(Ambient Lighting)：即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色。
漫反射光照(Diffuse Lighting)：模拟光源对物体的方向性影响(Directional Impact)。它是冯氏光照模型中视觉上最显著的分量。物体的某一部分越是正对着光源，它就会越亮。
镜面光照(Specular Lighting)：模拟有光泽物体上面出现的亮点。镜面光照的颜色相比于物体的颜色会更倾向于光的颜色。

### 环境光照
把环境光照添加到场景里非常简单。我们用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，然后将最终结果作为片段的颜色，
```
void main() {
    //常量环境因子
    float ambientStrength = 0.1;
    //lightColor 为光的颜色
    vec3 ambient = ambientStrength * lightColor;
    //再乘以物体的颜色就是我们看到的颜色
    FragColor = vec4(ambient * objectColor, 1.0);
}
```
### 漫反射光照
漫反射光照使物体上与光线方向越接近的片段能从光源处获得更多的亮。
法向量的概念：垂直于平面的直线所表示的向量为该平面的法向量。即下图的黄色箭头向量N，它是垂直于平面向外的。
当我们的光源按照法向量的反方向照射物体的时候反射光更亮，而垂直于法向量照射的时候反射光基本上看不到，也就是说θ越大，反射光的影响越小。
下图中的FragPos是当前光照射到物体的某一个片段上的位置。
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/9-0.png" width="50%">

如何计算？
先获取光的方向和法向量，这里法向量是通过顶点数据传给顶点着色器，然后顶点着色器又传给偏远着色器来用于计算。
光的方向向量，是指`片段位置`指向`光源位置`的向量。所以使用光源位置向量减去片段位置向量。
>如果还想不明白，可以思考一个摄像机位置为(0,0,3),摄像机指向原点(0,0,0)的方向,那么这个方向向量就是原点向量减去摄像机位置的向量(0,0,3)
计算光照我们只关心方向，不关心大小，可以通过normalize进行标准化。
```
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
```
对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫反射影响。
两个向量之间的角度越大，漫反射分量就会越小，参考 chapter 5中向量的点乘，两个向量点乘的结果是夹角的余弦值，夹角越小，反射分量越大，反之越小。
如果两个向量之间的角度大于90度，点乘的结果就会变成负数，负数颜色的光照是没有定义的，可以通过MAX函数保证取值不为负数。
结果值再乘以光的颜色，得到漫反射分量。
```
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;
```
现在我们有了环境光分量和漫反射分量，我们把它们相加，然后把结果乘以物体的颜色，来获得片段最后的输出颜色。
```
FragColor = vec4((ambient + diffuse) * objectColor, 1.0);
```

>最后对于法向量还需要进行一个处理
在片段着色器中，如果进行了不等比缩放，法向量就不会再垂直于对应的表面了，这样光照就会被破坏。
这里一个修复的技巧是使用法线矩阵，它使用了一些线性代数的操作来移除对法向量错误缩放的影响。
这里只说下需要调用的函数
```
void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;
    ...
}
```
### 镜面光照
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/9-1.png" width="50%">
镜面光照也是依据光的方向向量和物体的法向量来决定的，但是它也依赖于观察方向，例如玩家是从什么方向看着这个片段的。它的作用效果就是，当我们去看光被物体所反射的那个方向的时候，我们会看到一个高光。
上图中 的橙色R就是通过光的方向向量和法向量来计算出来的，光源和法向量N的夹角 ，R和N的夹角是一样的。观察位置和反射向量R的夹角越小，那么镜面光的影响就会越大。

在fragmentShader中添加viewPos，这里我们通过之前的FPS摄像机 的位置向量作为了观察向量。
```
uniform vec3 viewPos;
```
我们定义一个镜面强度(Specular Intensity)变量，给镜面高光一个中等亮度颜色，让它不要产生过度的影响。
```
float specularStrength = 0.5;
```
视线方向向量就是片段位置指向观察位置的向量。
reflect函数要求第一个向量是从光源指向片段位置的向量,而之前计算`lightDir = normalize(lightPos - FragPos)`是片段位置指向光源位置的向量，所以进行了取反。
将对应的沿着法线轴的反射向量和观察向量进行点乘，使用MAX保证不为负值。
不同的物体，有不同的反光度，比如木头和镜子，木头基本上看不到高光，而镜子高光很强烈。这里设置了32。pow是结果的32次幂。
```
vec3 viewDir = normalize(viewPos-FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //32代表的是一个物体的反光度，越高，反光能力越强。
```
现在我们有了环境光分量和漫反射分量和镜面反射分量，我们把它们相加，然后把结果乘以物体的颜色，来获得片段最后的输出颜色。
```
FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
```
下边显示了不同反光度显示的结果。
一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/9-2.png" width="50%">

## 10.材质
基础光照中讲了一个光照模型，也就是冯氏光照模型。
它的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。
除了光源可以从这三方面考虑。一个物体的材质也是需要从这三方面考虑的。

## 11.光照贴图
其实就是添加了纹理
## 12.投光物
### 定向光
也叫平行光（太阳）。假设光源处于无限远处的模型时，看起来好像所有的光都来自于同一个方向。
### 点光源。
从它的光源位置开始朝着所有方向散射光线。存在一个衰减的定义：随着光线传播距离的增长逐渐削减光的强度。
计算公式：
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/12-0.png" width="50%">
每一个光照模型的计算结果需要乘以这个衰减因子。
常数项Kc(一般为1.0)、一次项Kl和二次项Kq。 d为片段位置到光源位置的距离通过`length(light.position - fragPos)`函数来计算。
下边的表提供了一些模拟取值
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/12-1.png" width="50%">
### 聚光
聚光是位于环境中某个位置的光源，它只朝一个特定方向而不是所有方向照射光线。这样的结果就是只有在聚光方向的特定半径内的物体才会被照亮，其它的物体都会保持黑暗。聚光很好的例子就是路灯或手电筒。
聚光需要对边缘部分进行平缓/软化的处理。[参考](https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/)
## 13.多光源
其实就是对多个光源的光照模型的结果进行相加（影响的因子是相乘如衰减，多个光源的叠加结果是相加）。


# 模型加载
## 14.模型加载
### Assimp
模型文件文件格式有很多，它们之间通常并没有一个通用的结构。所以如果我们想从这些文件格式中导入模型的话，我们必须要去自己对每一种需要导入的文件格式写一个导入器。
Assimp（一个模型导入库）这个库就是专门处理这个问题的。

将导入的整个模型加载进一个场景（Scene）对象，它保健科导入的模型/场景中的所有数据。
Assimp将场景载入为一些列的结点（Node），每个节点包含了场景对象中存储数据的索引。

编译流程：`brew install assimp`
执行完后打开下边文件目录进行查看。
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/14-0.png" width="50%">
在工程中添加自定义路径${assmip_header}和${assmip_lib}，然后在build setting>header search 和 library search 中添加相应路径

### 网格类Mesh编写

### 导入3D模型
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/14-1.png" width="50%">

# 高级OpenGL
## 15.深度测试
通过`glad_glEnable(GL_DEPTH_TEST)`开启深度测试。
如果你启用了深度缓冲，你还应该每个渲染迭代之前使用`GL_DEPTH_BUFFER_BIT`来清除深度缓冲，否则你会仍在使用上一次渲染迭代中的写入的深度值：
```
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```
可以通过`glDepthFunc(GL_ALWAYS);`设置深度测试函数，后边的参数可选如下
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/15-2.png" width="50%">
默认为GL_LESS 在片段深度值小于缓冲的深度值时通过测试.
当这是为GL_ALWAYS的时候，深度测试将会永远通过，所以最后绘制的片段总是会渲染在 之前绘制的片段的上面。比如我们绘制两个箱子，放在地板上，因为我们是最后绘制地板，如果参数为GL_ALWAYS，那么地板的片段将会覆盖所有的箱子片段。
GL_LESS:
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/15-0.png" width="50%">
GL_ALWAYS：
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/15-1.png" width="50%">


⚠️对于深度值精度的计算这里概念还是很模糊。

## 16.模板测试
对于一个3D物体，我们在不同的视角看去，它都会有一个边缘,如下面两个图片中绿色所示：
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/16-0.png" width="30%"><img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/16-1.png" width="30%">
举一个我们最常见的关于它的使用场景，当我们玩儿一些RPG游戏的时候，用鼠标去点击一个NPC，这个NPC是一个选中状态，它周边会包围一个光圈，这个光圈一定是包围了当前这个3D的NPC所有的部分。其实就是所谓的物体轮廓。

上述就是模板测试的概念了。那么具体如何使用？
和深度测试一样通过`glad_glEnable(GL_STENCIL_TEST);`来开启模板测试。同样也要在每个渲染迭代之前使用`GL_STENCIL_BUFFER_BIT`来清楚模板缓冲。
```
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
```
除此之外还可通过另外两个函数，对模板缓冲应该通过还是失败，以及它应该如何影响模板缓冲，来进行一定控制。
>glStencilFunc(GLenum func, GLint ref, GLuint mask)
(*) func 设置模板测试函数 ，对已存储的模板值和后边的`ref`进行比较，可用选项和深度测试中的glDepthFunc方法的参数是一样的。
(*) ref 设置了模板测试的参考值(Reference Value)。模板缓冲的内容将会与这个值进行比较
(*) mask  设置一个掩码，它将会与参考值和储存的模板值在测试比较它们之前进行与(AND)运算。初始情况下所有位都为1。

比如下边这个简单的例子：
通过`glStencilFunc(GL_EQUAL, 1, 0xFF);`就可以进行控制，他会告诉OenGL，只要一个片段的模板值等于参考值1，片段就会通过测试并显示，否则丢弃。
>因为一个模板值是8位的，一共有256种模板值， 用十六进制表示就是0x00 ~ 0xFF，这里设置掩码为0xFF。当设置为0x00的时候没以为写入模板缓冲时都会变成0，等于禁用了写入。默认为0xFF。
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/16-2.png" width="50%">

`glStencilFunc`描述了OpenGL应该对模板缓冲内容做什么，而`glStencilOp`描述了应该如何更新缓冲。
>glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
(*) sfail 模板测试失败时采取的行为。
(*) dpfail 模板测试通过，但深度测试失败时采取的行为。
(*) dppass 模板测试和深度测试都通过时采取的行为。
参数的可选值如下：
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/16-3.png" width="50%">
默认情况下，三个参数都为`GL_KEEP`。即不修改模板缓冲。

下面简单说了下箱子加上边框的整体过程，请结合代码观看。
首先进行关于模板测试的初始配置，其实glad_glStencilFunc函数在这里的设置注释掉并没有影响整体运行后的效果，个人猜测该函数默认的设置也是GL_NOTEQUAL和1，0xFF为默认是知道的。
```
//    设置OpenGL的全局配置
//深度测试
glad_glEnable(GL_DEPTH_TEST);
glad_glDepthFunc(GL_LESS);//默认就为GL_LESS
//模板测试
glad_glEnable(GL_STENCIL_TEST);
glad_glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//如果其中的一个测试失败了，我们什么都不做，我们仅仅保留当前储存在模板缓冲中的值。
//如果模板测试和深度测试都通过了，那么我们希望将储存的模板值设置为参考值
glad_glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
```
这里我们调整了绘制的顺序，先来绘制了地板。
因为我们只对两个箱子加上边框,地板还是正常绘制，所以需要让地板禁用模板缓冲的写入，通过设置掩码0x00来实现。
```
glad_glStencilMask(0x00);
[绘制地板的代码]
```
这里开始进行边框绘制，这里可能会有人误以为我们只是单独绘制了边框，那么是不是需要一套边框的顶点坐标等等。。。其实这里的实现方式，也是绘制了一个立方体，只不过将这个立方体稍微放大了一点，然后这个边框立方体的 原点和箱子的原点是一样的，如果不做任何处理，我们可以想像一下我们是将一个小箱子放到了一个大箱子里了。
先来绘制我们的箱子,绘制之前进行模板测试的设置，让箱子整体都通过模板测试。ref为1,缓冲的更新策略为GL_REPLACE,所以只要深度和模板都通过测试，就将模板值设为了1，启动模板缓冲写入。
```
glad_glStencilFunc(GL_ALWAYS, 1, 0xFF);
glad_glStencilMask(0xFF);
【绘制两个箱子的代码】
```
>启动/关闭写入 你其实可以想象成对箱子的一个采样过程，后边绘制边框立方体是需要这个采样结果，并不需要再次采样。
绘制边框的立方体，因为之前对整个的箱子设置了模板缓冲，并将模板值设为了1，接下来只要模板值不为1才对边框立方体 进行绘制。
绘制完之后 还需要将一些设置还原为初始值。
```
glad_glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glad_glStencilMask(0x00);
//关闭深度测试来绘制边框，
glad_glDisable(GL_DEPTH_TEST);
【绘制边框的立方体】
//还原模板掩码的初始值
glad_glStencilMask(0xFF);
//重新允许深度测试
glad_glEnable(GL_DEPTH_TEST);
```
这里在绘制边框立方体的时候，因为对立方体进行了放大，那么对于底部的边框绘制一定会在地板下面，这时，我们在绘制之前先关闭深度测试，绘制之后再还原，就解决了这个问题。因为当你关闭深度测试之后，后绘制的内容不会去判断深度，直接会覆盖为最新的绘制内容。

出现的问题：
1.如果我试图后绘制地板，会发现整个地板的纹理消失了，全部为边框立方体的绿色。这是为什么？
因为这里有两个shader，你之前绘制箱子和地板用的shader，通过glad_glUseProgram进行了激活，当你绘制边框箱子的时候你又激活了另外一个shaderSingleColor。着色器源代码更换了。

## 17.混合
混合其实就是细线物体透明度的一种技术。
### 1.部分片段全透明 
有些图片并不需要半透明，它的alpha通道在某一个片段就是透明的，我们可以直接在片段着色器中通过alpha通道值的判断，然后使用`discard`直接丢弃该片段，不再对它进行处理。
注意，当采样纹理的边缘的时候，OpenGL会对边缘的值和纹理下一个重复的值进行插值（因为我们将它的环绕方式设置为了GL_REPEAT。这通常是没问题的，但是由于我们使用了透明值，纹理图像的顶部将会与底部边缘的纯色值进行插值。这样的结果是一个半透明的有色边框，你可能会看见它环绕着你的纹理四边形。要想避免这个，每当你alpha纹理的时候，请将纹理的环绕方式设置为GL_CLAMP_TO_EDGE。下图就是未设置的结果
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/17-0.png" width="50%">

### 2.1 半透明。
首先需要开启混合 `glad_glEnable(GL_BLEND)` 。
它的计算方法如下：
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/17-2.png" width="50%">
混合也有一些相关函数。
>glBlendFunc(GLenum sfactor, GLenum dfactor)
(*)sfactor 源因子值。指定了alpha值对源颜色的影响。
(*)dfactor 目标因子值。指定了alpha值对目标颜色的影响。
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/17-1.png" width="50%">
注意：常数颜色向量C¯constant可以通过glBlendColor函数来另外设置。

也可以使用`glBlendFuncSeparate`函数为RGB和alpha通道分别设置不同的选项。如：
```
glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
```
除此之外`glBlendEquation(GLenum mode)`函数还可以设置运算符
(*)GL_FUNC_ADD：默认选项，将两个分量相加：C¯result=Src+Dst。
(*)GL_FUNC_SUBTRACT：将两个分量相减： C¯result=Src−Dst。
(*)GL_FUNC_REVERSE_SUBTRACT：将两个分量相减，但顺序相反：C¯result=Dst−Src。
通常我们都可以省略调用glBlendEquation，因为GL_FUNC_ADD对大部分的操作来说都是我们希望的混合方程

### 2.2 绘制顺序

绘制一个有不透明和透明物体的场景的时候，大体原则：
1. 先绘制所有不透明的物体。
2. 对所有透明的物体排序，先绘制最远，再绘制最近。
3. 按顺序绘制所有透明的物体。

排序透明物体的一种方法是，从观察者视角获取物体的距离。这可以通过计算摄像机位置向量和物体的位置向量之间的距离所获得。接下来我们把距离和它对应的位置向量存储到一个STL库的map数据结构中。map会自动根据键值(Key)对它的值排序，所以只要我们添加了所有的位置，并以它的距离作为键，它们就会自动根据距离值排序了。
