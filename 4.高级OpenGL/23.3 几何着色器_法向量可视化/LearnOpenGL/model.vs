#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec2 texCoords;
    vec3 fragPos;
    vec3 normal;
} vs_out;
void main()
{
    
    vs_out.texCoords = aTexCoords;
    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(vs_out.fragPos, 1.0);

}
