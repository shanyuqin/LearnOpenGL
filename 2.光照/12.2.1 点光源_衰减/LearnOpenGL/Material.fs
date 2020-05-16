#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    //聚光的 三个属性 位置，方向，切光角
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;
    
    //光照模型属性
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    //衰减属性
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    
    //环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
   
    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , TexCoords));
   
    //镜面高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular , TexCoords)) ;
   
    //聚光 （平滑）
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    //衰减
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   
    FragColor = vec4((ambient + diffuse + specular)*attenuation, 1.0);
   
    
    
    
}
