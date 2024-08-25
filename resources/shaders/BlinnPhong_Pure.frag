#version 460 core
out vec4 FragColor;

struct DirLight{
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight{
    vec3 position;
    vec3 color;
    float intensity;
};

in VS_OUT{
    vec3 fragPos_wS;
    mat3 TBN;
    vec3 normal_wS;
    vec2 texCoord;
} fs_in;
// set by render component
layout(std140,binding=1) uniform Camera{
    vec3 cameraPos;
};

layout(std140,binding=2) uniform Lights{
    DirLight sunLight;
};

// set by material
uniform float ambient;
uniform vec3 color;

vec3 CalcDirLight(vec3 lightDir,vec3 lightColor,float intensity){
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos_wS);
    vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
    float specular = max(dot(halfwayDir,fs_in.normal_wS),0.0);
    float diffuse = max(dot(lightDir,fs_in.normal_wS),0.0);
    return (specular + diffuse) * lightColor * intensity;
}

vec3 CalcPointLight(vec3 lightPos,vec3 lightColor,float intensity){
    float distance = length(lightPos - fs_in.fragPos_wS);
    float decay = 1.0 / (distance * distance);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos_wS);
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos_wS);
    vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
    float specular = max(dot(halfwayDir,fs_in.normal_wS),0.0);
    float diffuse = max(dot(lightDir,fs_in.normal_wS),0.0);
    return (specular + diffuse) * lightColor * intensity * decay;
}

// set by drawable itself
uniform sampler2D diffuse1;
void main(){
    vec3 albedo = color * ambient;
    albedo += CalcDirLight(-sunLight.direction,sunLight.color,sunLight.intensity);
    FragColor = vec4(albedo,1.0);
}


