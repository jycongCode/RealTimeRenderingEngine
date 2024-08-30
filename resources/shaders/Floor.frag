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
uniform vec3 color;

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


#define NUM_SAMPLES 50
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 2e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

vec2 poissonDisk[NUM_SAMPLES];

highp float rand_2to1(vec2 uv ) {
    // 0 - 1
    float a = 12.9898, b = 78.233, c = 43758.5453;
    float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
    return fract(sin(sn) * c);
}

void poissonDiskSamples( vec2 randomSeed ) {

    float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
    float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

    float angle = rand_2to1( randomSeed ) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

float findBlocker(vec2 uv, float zReceiver ) {
    poissonDiskSamples(uv);
    // uniformDiskSamples(uv);
    float distanceReceiver = zReceiver;
    float distanceBlocker = 0.0;
    int num = 0;
    for(int i = 0; i < NUM_SAMPLES; ++i) {
        vec2 coord = uv + 0.25 * zReceiver * poissonDisk[i];
        if(coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0) {
            continue;
        }
        float d = texture2D(shadowMap, coord).r;
        if(d < zReceiver - EPS) {
            distanceBlocker += d;
            num += 1;
        }
    }
    if(num != 0) {
        distanceBlocker /= float(num);
    }
    return max(0.4, (distanceReceiver - distanceBlocker) / distanceBlocker);
}

float useShadowMap(vec4 shadowCoord){
    if(shadowCoord.x < 0.0 || shadowCoord.x > 1.0 || shadowCoord.y < 0.0 || shadowCoord.y > 1.0) {
        return 1.0;
    }
    return step(shadowCoord.z, texture2D(shadowMap, shadowCoord.xy).r + EPS);
}

float PCSS(vec4 coords){
    if(coords.x < 0.0 || coords.x > 1.0 || coords.y < 0.0 || coords.y > 1.0) {
        return 1.0;
    }
    float filterKernelArea = findBlocker(coords.xy, coords.z);
    poissonDiskSamples(coords.xy);
    // uniformDiskSamples(coords.xy);
    float visibility = 0.0;
    for(int i = 0; i < NUM_SAMPLES; ++i) {
        vec4 c = vec4(coords.xy + filterKernelArea * 0.002 * poissonDisk[i], coords.zw);
        visibility += useShadowMap(c);
    }
    return visibility / float(NUM_SAMPLES);
}

float CalcVisibility(vec4 lightSpacePos,vec3 normal,vec3 lightDir){
    vec4 projCoord = lightSpacePos.xyzw / lightSpacePos.w;
    projCoord = projCoord * 0.5 + 0.5;
    return PCSS(projCoord);
//    return useShadowMap(projCoord);
    //    float blockerDepth = texture(shadowMap,projCoord.xy).r;
    //    float bias = max(biasMax * (1.0 - dot(normal, lightDir)), biasMin);
    //    return blockerDepth < projCoord.z-bias ? 0.0 : 1.0;
}

void main(){
    // ambient light
    vec3 indirectLight = color * ambient;
    vec3 directLight = CalcDirLight(-sunLight.direction,sunLight.color,sunLight.intensity);
    float visibility = CalcVisibility(fs_in.fragPos_lS,fs_in.normal_wS,-sunLight.direction);
    FragColor = vec4((indirectLight + directLight * visibility) * color,1.0);
}


