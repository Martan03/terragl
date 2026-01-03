#version 460 core
layout(location = 0) in vec3 pos;

out vec3 texCoord;

uniform mat4 view;
uniform mat4 proj;

void main() {
    texCoord = pos;
    vec4 tpos = proj * view * vec4(pos, 1.0);
    gl_Position = tpos.xyww;
}
