
#version 330 core
out vec4 Color;
uniform float col;
void main(){
    Color = vec4(1.0,col,0.0,1.0);
}
