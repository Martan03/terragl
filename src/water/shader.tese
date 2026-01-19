#version 460 core
layout(quads, fractional_odd_spacing, ccw) in;

in vec3 tPos[];
in vec2 tUv[];

out float depth;
out vec3 fragPos;
out vec4 fragPosLight;
out vec3 fragNormal;

uniform sampler2D heightTex;
uniform sampler2D waterTex;
uniform mat4 lightMat, model, view, proj;
uniform float amp;
uniform float time;

struct Wave {
    vec2 dir;
    float freq;
    float amp;
    float speed;
};

vec3 getWave(vec2 pos, float t) {
    vec3 result = vec3(0.0);

    Wave waves[6];
    waves[0] = Wave(normalize(vec2(1.0, 0.2)), 1.4, 0.030, 1.1);
    waves[1] = Wave(normalize(vec2(-0.4, 0.8)), 2.2, 0.020, 1.5);
    waves[2] = Wave(normalize(vec2(0.3, -0.7)), 3.1, 0.015, 2.2);
    waves[3] = Wave(normalize(vec2(-0.8, -0.1)), 1.1, 0.035, 0.7);
    waves[4] = Wave(normalize(vec2(0.1, 1.0)), 4.5, 0.008, 3.0);
    waves[5] = Wave(normalize(vec2(-0.9, -0.6)), 0.7, 0.045, 0.5);

    for (int i = 0; i < 6; i++) {
        vec2 perpDir = vec2(-waves[i].dir.y, waves[i].dir.x);
        float wobble = sin(dot(pos, perpDir) * 0.1) * 2.0;

        float phase = dot(pos, waves[i].dir) + wobble;
        phase = phase * waves[i].freq + t * waves[i].speed;
        float s = pow(sin(phase) * 0.5 + 0.5, 1.5);
        float c = cos(phase);

        result.x += waves[i].amp * s;
        float d = waves[i].amp * waves[i].freq * c * 1.5;
        d *= sqrt(abs(sin(phase) * 0.5 + 0.5));
        result.y += waves[i].dir.x * d;
        result.z += waves[i].dir.y * d;
    }

    return result;
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = mix(tPos[0], tPos[1], u);
    vec3 p1 = mix(tPos[3], tPos[2], u);
    vec3 pos = mix(p0, p1, v);

    vec2 uv0 = mix(tUv[0], tUv[1], u);
    vec2 uv1 = mix(tUv[3], tUv[2], u);
    vec2 uv = mix(uv0, uv1, v);

    float waterBaseLevel = texture(waterTex, uv).r;
    float terrainHeight = texture(heightTex, uv).r;

    vec3 wave = vec3(0.0);
    if (waterBaseLevel > -amp) {
        wave = getWave(pos.xz, time);
    }

    float finalH = waterBaseLevel + wave.x;
    depth = finalH - terrainHeight;
    fragNormal = normalize(vec3(-wave.y, 1.0, -wave.z));

    fragPos = vec3(model * vec4(pos.x, finalH, pos.z, 1.0));
    gl_Position = proj * view * vec4(fragPos, 1.0);
}
