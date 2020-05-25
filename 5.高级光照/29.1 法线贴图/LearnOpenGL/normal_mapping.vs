#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos; //切线空间下光的位置
    vec3 TangentViewPos; //切线空间下视角位置
    vec3 TangentFragPos; //切线空间下片段位置
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos,1.0);
    vs_out.FragPos = vec3(model * vec4(aPos , 1.0));
    vs_out.TexCoords = aTexCoords;
    
    mat3 normalMat = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMat * aTangent);
    vec3 B = normalize(normalMat * aBitangent);
    vec3 N = normalize(normalMat * aNormal);
    
    mat3 TBN = transpose(mat3(T,B,N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;
}
