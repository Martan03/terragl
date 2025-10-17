#version 460 core
layout(vertices = 4) out;

in vec3 vPos[];
in vec3 vNorm[];

out vec3 tPos[];
out vec3 tNorm[];

void main() {
    tPos[gl_InvocationID] = vPos[gl_InvocationID];
    tNorm[gl_InvocationID] = vNorm[gl_InvocationID];

    gl_TessLevelOuter[0] = 4;
    gl_TessLevelOuter[1] = 4;
    gl_TessLevelOuter[2] = 4;
    gl_TessLevelOuter[3] = 4;

    gl_TessLevelInner[0] = 4;
    gl_TessLevelInner[1] = 4;
}
