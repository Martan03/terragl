#version 460 core
layout(quads, fractional_odd_spacing, ccw) in;

in vec3 tPos[];
in vec2 tUv[];

out float depth;
out vec3 fragPos;

uniform sampler2D heightTex;
uniform sampler2D waterTex;
uniform mat4 model, view, proj;
uniform float amp;
// uniform float time;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = mix(tPos[0], tPos[1], u);
    vec3 p1 = mix(tPos[3], tPos[2], u);
    vec3 pos = mix(p0, p1, v);

    vec2 uv0 = mix(tUv[0], tUv[1], u);
    vec2 uv1 = mix(tUv[3], tUv[2], u);
    vec2 uv = mix(uv0, uv1, v);

    float height = texture(heightTex, uv).r;
    float wdepth = texture(waterTex, uv).r;

    // // Displacement logic: simple Gerstner wave approximation
    // // float wave = sin(uv.x * 20.0 + time) * cos(uv.y * 20.0 + time) * 0.2;

    depth = wdepth - height;
    fragPos = vec3(model * vec4(pos.x, wdepth, pos.z, 1.0));
    gl_Position = proj * view * vec4(fragPos, 1.0);
}
