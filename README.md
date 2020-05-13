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
![image](https://raw.githubusercontent.com/shanyuqin/LearnOpenGL/master/ReadMeImage/4-0.png)
    
