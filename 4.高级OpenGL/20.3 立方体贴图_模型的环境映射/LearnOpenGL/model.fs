#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 cameraPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_shininess1;
uniform sampler2D texture_ambient1;

uniform vec3 viewPos;

void main()
{

    FragColor = vec4(texture(texture_ambient1,TexCoords).rgb,1.0);

//    vec3 viewDir = normalize(FragPos - cameraPos);//从观察位置指向片段位置
//    //反射
//    vec3 reflectDir = reflect(viewDir,normalize(Normal));
//    FragColor = vec4(texture(texture_ambient1,reflectDir).rgb,1.0);
}
