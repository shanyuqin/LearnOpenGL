#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
//GLSL有一个供纹理对象使用的内建数据类型，叫做采样器
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
//    (80% container, 20% awesomeface)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2( -TexCoord.x, TexCoord.y)), 0.2);
}
