#version 460 core
layout(vertices = 4) out;

in vec3 vPos[];
in vec2 vUv[];

out vec3 tPos[];
out vec2 tUv[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    tPos[gl_InvocationID] = vPos[gl_InvocationID];
    tUv[gl_InvocationID] = vUv[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        const int MIN_TESS_LEVEL = 2;
        const int MAX_TESS_LEVEL = 8;
        const float MIN_DISTANCE = 20.0;
        const float MAX_DISTANCE = 100.0;

        vec4 eyePos0 = view * model * vec4(vPos[0], 1.0);
        vec4 eyePos1 = view * model * vec4(vPos[1], 1.0);
        vec4 eyePos2 = view * model * vec4(vPos[2], 1.0);
        vec4 eyePos3 = view * model * vec4(vPos[3], 1.0);

        float dist0 = length(eyePos0.xyz);
        float dist1 = length(eyePos1.xyz);
        float dist2 = length(eyePos2.xyz);
        float dist3 = length(eyePos3.xyz);

        dist0 = clamp((dist0 - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        dist1 = clamp((dist1 - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        dist2 = clamp((dist2 - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        dist3 = clamp((dist3 - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        float tessLevel0 = mix(float(MAX_TESS_LEVEL), float(MIN_TESS_LEVEL), min(dist2, dist0));
        float tessLevel1 = mix(float(MAX_TESS_LEVEL), float(MIN_TESS_LEVEL), min(dist0, dist1));
        float tessLevel2 = mix(float(MAX_TESS_LEVEL), float(MIN_TESS_LEVEL), min(dist1, dist3));
        float tessLevel3 = mix(float(MAX_TESS_LEVEL), float(MIN_TESS_LEVEL), min(dist3, dist2));

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}
