#version 450

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec2 textCoord;

layout(push_constant)uniform pushConstants{
    mat4 model;
    mat4 viewProj;
}pc;




void main() {
    
    gl_Position = pc.model * pc.viewProj * vec4(pos, 0.0f ,1.0f); 
    
    textCoord = uv;
}