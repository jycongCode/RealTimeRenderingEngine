#version 460 core
layout (location = 0) in vec3 vertexPos_mS; // the position variable has attribute position 0

uniform mat4 lightMatrix;
uniform mat4 model;
void main(){
    gl_Position = lightMatrix * model * vec4(vertexPos_mS,1.0);
}
