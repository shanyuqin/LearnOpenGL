
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float time;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * 0.2;
    EmitVertex();
    EndPrimitive();
    //在一个或多个EmitVertex调用之后重复调用EndPrimitive能够生成多个图元
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * 5.0;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * 6.0;
    EmitVertex();
    EndPrimitive();

}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}
