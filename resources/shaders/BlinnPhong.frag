#version 450 core
out vec4 FragColor;

in VS_OUT{
    vec3 fragPos_tS;
    vec3 cameraPos_tS;
    vec3 dirLight_tS;
//    vec3 pointLight_tS[4];
    vec3 fragPos_wS;
    vec3 cameraPos_wS;
    vec2 texCoord;
//    vec4 fragPos_lS;
} fs_in;

uniform sampler2D diffuse1;
uniform vec3 lightColor;
void main(){
    vec3 albedo = texture(diffuse1,fs_in.texCoord).rgb;
    vec3 ambient = albedo * 0.5;

    vec3 normal = vec3(0.0,0.0,1.0);
    vec3 lightDir = normalize(-fs_in.dirLight_tS);
    vec3 viewDir = normalize(fs_in.cameraPos_tS - fs_in.fragPos_tS);
    vec3 halfwayDir = normalize((lightDir+viewDir)/2.0);
    float specular = max(dot(halfwayDir,normal),0.0);

    float diffuse = max(dot(lightDir,normal),0.0);

    vec3 color = ambient + (diffuse+specular) * lightColor * 0.5;

    FragColor = vec4(color,1.0);
}