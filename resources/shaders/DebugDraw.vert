#version 460 core
layout (location = 0) in vec3 vertexPos_mS;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
void main(){
    gl_Position = vec4(vertexPos_mS,1.0);
    TexCoord = aTexCoord;
}