#version 330 core
out vec4 FragColor;
in vec2 myPos;
in float Speed;

void main() {
    //FragColor = vec4(vec3(cos(myPos.y*15-3+sin(myPos.x*15))),1.0);return;
    if (Speed==0.0) {
        FragColor=vec4(1.0);
    } else if (Speed<=1.0) {
        float factor = Speed;
        FragColor=(1.0-factor)*vec4(vec3(0,255,33)/255,1)+factor*vec4(vec3(255,222,0)/255,1);
    } else {
        float factor = Speed-1.0;
        FragColor=(1.0-factor)*vec4(vec3(255,222,0)/255,1)+factor*vec4(vec3(255,0,0)/255,1);
    }
}