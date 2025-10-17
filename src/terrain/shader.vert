#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

out vec3 vPos;
out vec3 vNorm;

void main() {
    vPos = pos;
    vNorm = norm;
}
