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

float CalcVisibility(vec4 lightSpacePos){
    vec3 projCoord = lightSpacePos.xyz / lightSpacePos.w;
    projCoord = projCoord * 0.5 + 0.5;
    float blockerDepth = texture(shadowMap,projCoord.xy).r;
    return blockerDepth < projCoord.z-0.01 ? 0.0 : 1.0;
}

void main(){
    // ambient light
    vec3 color = texture(diffuse1,fs_in.texCoord).rgb;
    vec3 indirectLight = texture(diffuse1,fs_in.texCoord).rgb * ambient;
    vec3 directLight = CalcDirLight(-sunLight.direction,sunLight.color,sunLight.intensity);
    float visibility = CalcVisibility(fs_in.fragPos_lS);
    FragColor = vec4((indirectLight + directLight * visibility) * color ,1.0);
}



