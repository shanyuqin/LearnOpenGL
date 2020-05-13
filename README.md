# LearnOpenGL
[LearnOpenGL CN](https://learnopengl-cn.github.io)的学习笔记

### OpenGL01——环境搭建并创建一个窗口
### OpenGL02——画一个橙色三角形
### OpenGL03——着色器uniform的使用
主要介绍uniform的使用。
### OpenGL03——为着色器添加更多属性
为顶点着色器添加了一个颜色属性。每一个顶点就包括了六个数据分别是 XYZRGB
定义顶点着色器的时候看到的layout （location = 0） in vec3 XXX;这里的location其实就是这个顶点向量的一个相对位置。
具体可以看代码中顶点着色器源代码的注释。
### OpenGL03——定义自己的着色器类
通过字符串编写着色器的源代码，然后在进行编译和管理，比较麻烦，这里写了一个类来从硬盘中读取着色器，然后编译并链接他们，以及进行错误检查。
但是对于ifstream的文件读取这里只能使用绝对路径么？这里是一个疑问
### OpenGL04——纹理
对于纹理渲染使用的图片，要想达到官网上的例子效果，那么图片必须是无alpha通道。
有alpha通道的情况是这样的
<img src="https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/4-0.png" width="50%">

### OpenGL05——矩阵/变换/向量/GLM
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
线性代数的内容可以参考[【官方双语/合集】线性代数的本质 - ](https://www.bilibili.com/video/BV1ys411472E)
