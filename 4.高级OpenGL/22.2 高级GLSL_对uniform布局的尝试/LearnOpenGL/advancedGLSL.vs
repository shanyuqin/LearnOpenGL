#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices {
    float float1;
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
out float Float1;

void main()
{
    Float1 = float1;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
