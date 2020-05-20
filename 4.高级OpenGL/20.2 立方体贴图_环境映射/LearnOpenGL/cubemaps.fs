#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    
    vec3 viewDir = normalize(FragPos - cameraPos);//从观察位置指向片段位置
    //反射
    vec3 reflectDir = reflect(viewDir,normalize(Normal));
    FragColor = vec4(texture(skybox,reflectDir).rgb,1.0);

    //折射
    float ratio = 1.00 / 1.52;
    vec3 refractDir = refract(viewDir, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox,refractDir).rgb,1.0);
}
