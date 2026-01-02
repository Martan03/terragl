#version 460 core
layout(quads, fractional_odd_spacing, ccw) in;

in vec3 tPos[];
in vec3 tNorm[];
in vec2 tUv[];

out vec3 fragPos;
out vec3 normal;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform sampler2D tex;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = mix(tPos[0], tPos[1], u);
    vec3 p1 = mix(tPos[3], tPos[2], u);
    vec3 pos = mix(p0, p1, v);

    vec3 n0 = mix(tNorm[0], tNorm[1], u);
    vec3 n1 = mix(tNorm[3], tNorm[2], u);
    vec3 n = normalize(mix(n0, n1, v));

    vec2 uv0 = mix(tUv[0], tUv[1], u);
    vec2 uv1 = mix(tUv[3], tUv[2], u);
    uv = mix(uv0, uv1, v);

    vec4 col = texture(tex, uv);
    fragPos = vec3(model * vec4(pos.x, col.r, pos.z, 1.0));
    normal = n;
    // normal = mat3(transpose(inverse(model))) * n;

    gl_Position = proj * view * vec4(fragPos, 1.0);
}
