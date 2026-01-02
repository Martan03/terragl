#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

out vec3 vPos;
out vec2 vUv;

void main() {
    vPos = pos;
    vUv = uv;
}
