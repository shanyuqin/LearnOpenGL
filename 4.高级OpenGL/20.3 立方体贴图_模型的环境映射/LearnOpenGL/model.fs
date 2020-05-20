#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_ambient1;

void main()
{

    vec3 viewDir = normalize(FragPos - cameraPos);//从观察位置指向片段位置
//    反射
//    vec3 reflectDir = reflect(viewDir,normalize(Normal));
//    FragColor = vec4(texture(skybox,reflectDir).rgb + ambient + diffuse,1.0);
//    折射
//    float ratio = 1.00 / 1.52;
//    vec3 refractDir = refract(viewDir, normalize(Normal), ratio);
//    FragColor = vec4(texture(skybox,refractDir).rgb,1.0);
           
   //漫反射
    vec3 diffuse =  vec3(texture(texture_diffuse1 , TexCoords));
              
    //镜面高光
    vec3 norm = normalize(Normal);
    vec3 reflectDir = reflect(viewDir,norm);
    vec3 specular =  vec3(texture(texture_specular1 , TexCoords)) ;
    
    FragColor = vec4(( diffuse + specular )* texture(skybox,reflectDir).rgb, 1.0);
}
