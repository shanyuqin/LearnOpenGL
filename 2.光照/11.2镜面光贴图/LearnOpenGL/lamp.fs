#version 330 core
out vec4 FragColor;

uniform vec3 lampColor;
void main()
{
//    FragColor = vec4(lampColor,1.0); //光源颜色
    FragColor = vec4(1.0); //白色光源
}
