#version 330 core
layout (location=0) in vec3 aPos;
out vec2 myPos;
out float Speed;

void main() {
    gl_Position=vec4(aPos.x,aPos.y,1.0,1.0);
    Speed = aPos.z;
    myPos = aPos.xy;
}