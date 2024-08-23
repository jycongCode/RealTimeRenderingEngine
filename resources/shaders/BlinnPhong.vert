#version 460 core

layout (location = 0) in vec3 vertexPos_mS; // the position variable has attribute position 0
layout (location = 1) in vec3 normal_mS;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 tangent_mS;


layout (std140,binding=0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

out VS_OUT{
    vec3 fragPos_wS;
    mat3 TBN;
    vec3 normal_wS;
    vec2 texCoord;
} vs_out;

void main(){
    //Position in clip space
    mat4 MVP = projection * view * model;
    gl_Position = MVP * vec4(vertexPos_mS, 1.0);

    //Passing texture coords
    vs_out.texCoord = aTexCoord;

    //World space output
    vs_out.fragPos_wS    =  vec3(M * vec4(vertexPos_mS, 1.0));

    //Generating tangent matrix
    vec3 T = normalize(mat3(M) * tangent_mS);
    vec3 N = normalize(mat3(M) * normal_mS);
    T = normalize(T - dot(T, N ) * N);
    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);

    vs_out.normal_wS = mat3(transpose(inverse(model))) * normal_mS;
}