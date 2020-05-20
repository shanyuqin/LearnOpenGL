#version 330 core
out vec4 FragColor;

in float Float1;

void main()
{
    FragColor = vec4(1.0 * Float1, 0.0, 0.0, 1.0);
}
