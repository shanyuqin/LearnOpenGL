#version 330 core
in vec3 Normal;
in vec3 FragPos;//顶点着色器传过来的物体的位置，因为可能会存在转换，vs通过转换计算后 传给fs

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;


void main()
{
    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //漫反射光照
    vec3 norm = normalize(Normal);
//灯光方向向量 （也就是从被照射的片段指向灯光或者说从片段看向灯光 计算是灯的位置 - 片段位置）
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    //镜面光照
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //32代表的是一个物体的反光度，越高，反光能力越强。
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
    
}
