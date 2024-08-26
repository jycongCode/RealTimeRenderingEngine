#version 460 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D debugTex;
void main(){
    float depth = texture(debugTex,TexCoord).r;
    FragColor = vec4(depth,depth,depth,1.0);
}