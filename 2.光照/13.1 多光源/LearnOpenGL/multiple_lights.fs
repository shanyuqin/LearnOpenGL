#version 330 core
out vec4 FragColor;
//材质
struct Material {
    //    vec3 ambient; 移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存
    sampler2D diffuse; // 漫反射采样
    sampler2D specular; // 镜面反射采样
    float shininess;
};
//定向光 （太阳）
struct DirLight {
    vec3 direction;
    
    vec3 ambient;//环境光照
    vec3 diffuse;//漫反射光照
    vec3 specular;//镜面光照
};
//点光源
struct PointLight {
    vec3 position;
    
    //点光源 衰减参数
    float constant;
    float liner;
    float quadratic;
    
    vec3 ambient;//环境光照
    vec3 diffuse;//漫反射光照
    vec3 specular;//镜面光照
    
};
//聚光 （手电筒）
struct SpotLight {
    vec3 position;//位置
    vec3 direction;//照射方向
    float cutOff;//切光角
    float outCutOff;//外切光角
    
    //点光源 衰减参数
    float constant;
    float liner;
    float quadratic;

    vec3 ambient;//环境光照
    vec3 diffuse;//漫反射光照
    vec3 specular;//镜面光照
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;//片段位置
in vec3 Normal;//法向量
in vec2 TexCoords;//纹理坐标

uniform vec3 viewPos;//观察位置
uniform DirLight dirLight;//定向光
uniform PointLight pointLights[NR_POINT_LIGHTS];//点光源数组
uniform SpotLight spotLight;//聚光光源
uniform Material material;//材质

//函数定义
vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = CalDirLight(dirLight, norm, viewDir);
    
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalPointLight(pointLights[i],norm, FragPos , viewDir);
    
    result += CalSpotLight(spotLight, norm, FragPos, viewDir);
    
    FragColor = vec4(result,1.0f);
}


vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //漫反射光照因子计算
    float diff = max(dot(normal, lightDir), 0.0);
    //镜面光照因子计算
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
    
    return ambient + diffuse + specular;
}
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //漫反射光照因子计算
    float diff = max(dot(normal, lightDir), 0.0);
    //镜面光照因子计算
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    //衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.liner * distance + light.quadratic * distance * distance);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
    
    return (ambient + diffuse + specular)*attenuation;
    
}
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    //漫反射光照因子计算
    float diff = max(dot(normal, lightDir), 0.0);
    //镜面光照因子计算
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    //衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.liner * distance + light.quadratic * distance * distance);
    //聚光
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilion = light.cutOff - light.outCutOff;
    float intensity = clamp((theta - light.outCutOff) / epsilion, 0.0, 1.0);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular) * attenuation * intensity;
    
}







