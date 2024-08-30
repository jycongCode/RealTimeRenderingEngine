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
    vec4 fragPos_lS;
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
// set by drawable itself
uniform sampler2D diffuse1;
uniform sampler2D shadowMap;

uniform float biasMin;
uniform float biasMax;
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

float CalcVisibility(vec4 lightSpacePos,vec3 normal,vec3 lightDir){
    vec3 projCoord = lightSpacePos.xyz / lightSpacePos.w;
    projCoord = projCoord * 0.5 + 0.5;
    if(projCoord.x < 0.0 || projCoord.x > 1.0 || projCoord.y < 0.0 || projCoord.y > 1.0)return 1.0;
    float blockerDepth = texture(shadowMap,projCoord.xy).r;
    float bias = max(biasMax * (1.0 - dot(normal, lightDir)), biasMin);
    return blockerDepth < projCoord.z-bias ? 0.0 : 1.0;
}

void main(){
    // ambient light
    vec3 color = pow(texture(diffuse1,fs_in.texCoord).rgb,vec3(2.2));
    vec3 indirectLight = texture(diffuse1,fs_in.texCoord).rgb * ambient;
    vec3 directLight = CalcDirLight(-sunLight.direction,sunLight.color,sunLight.intensity);
    float visibility = 1.0;
    FragColor = vec4(pow((indirectLight + directLight * visibility) * color,vec3(1.0/2.2)) ,1.0);
}



