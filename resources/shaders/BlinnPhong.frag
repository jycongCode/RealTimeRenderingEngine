#version 450 core
out vec4 FragColor;

layout(std140,binding=2) Lights{
    DirLight sunLight;
    float envLight;
};

in VS_OUT{
    vec3 fragPos_wS;
    mat3 TBN;
    vec3 normal_wS;
    vec2 texCoord;
} fs_in;

uniform sampler2D diffuse1;
void main(){
    vec3 albedo = texture(diffuse1,fs_in.texCoord).rgb * envLight;
    vec3 color = albedo;
    color += CalcDirLight(-sunLight.direction,sunLight.color,sunLight.intensity);
    FragColor = vec4(color,1.0);
}

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

vec3 CalcDirLight(vec3 lightDir,vec3 lightColor,vec3 intensity){
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos_wS);
    vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
    float specular = max(dot(halfwayDir,fs_in.normal_wS),0.0);
    float diffuse = max(dot(lightDir,fs_in.normal_wS),0.0);
    return (specular + diffuse) * lightColor * intensity;
}

vec3 CalcPointLight(vec3 lightPos,vec3 lightColor,vec3 intensity){
    float distance = length(lightPos - fs_in.fragPos_wS);
    float decay = 1.0 / (distance * distance);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos_wS);
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos_wS);
    vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
    float specular = max(dot(halfwayDir,fs_in.normal_wS),0.0);
    float diffuse = max(dot(lightDir,fs_in.normal_wS),0.0);
    return (specular + diffuse) * lightColor * intensity * decay;
}

