#version 450 core
out vec4 FragColor;

struct DirLight{
    vec3 color;
    float intensity;
};
uniform DirLight dirLights[5];
uniform int dirNum;

struct PointLight{
    vec3 color;
    float intensity;
    float radius;
};
uniform PointLight pointLights[5];
uniform int pointNum;

in VS_OUT{
    vec3 fragPos_tS;
    vec3 cameraPos_tS;
    vec3 dirLight_tS[5];
    vec3 pointLight_tS[5];
    vec3 fragPos_wS;
    vec3 cameraPos_wS;
    vec2 texCoord;
//    vec4 fragPos_lS;
} fs_in;

uniform sampler2D diffuse1;
uniform float envLight;
void main(){
    vec3 albedo = texture(diffuse1,fs_in.texCoord).rgb * envLight;
    vec3 normal = vec3(0.0,0.0,1.0);
    vec3 color = vec3(0.0,0.0,0.0);

    for(int i = 0;i<dirNum;++i){
        vec3 lightDir = normalize(-fs_in.dirLight_tS[i]);
        vec3 viewDir = normalize(fs_in.cameraPos_tS - fs_in.fragPos_tS);
        vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
        float specular = max(dot(halfwayDir,normal),0.0);
        float diffuse = max(dot(lightDir,normal),0.0);
        color += (diffuse+specular) * dirLights[i].color * dirLights[i].intensity;
    }

    for(int i = 0;i<pointNum;++i){
        float distance = length(fs_in.pointLight_tS[i] - fs_in.fragPos_tS);
        if(distance > pointLights[i].radius){
            continue;
        }
        float decay = 1.0 / (distance * distance);
        vec3 lightDir = normalize(fs_in.pointLight_tS[i] - fs_in.fragPos_tS);
        vec3 viewDir = normalize(fs_in.cameraPos_tS - fs_in.fragPos_tS);
        vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
        float specular = max(dot(halfwayDir,normal),0.0);
        float diffuse = max(dot(lightDir,normal),0.0);
        color += (diffuse+specular) * dirLights[i].color * dirLights[i].intensity * decay;
    }
    color += albedo;
    FragColor = vec4(color,1.0);
}