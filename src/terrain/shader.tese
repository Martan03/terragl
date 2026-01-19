#version 460 core
layout(quads, fractional_odd_spacing, ccw) in;

in vec3 tPos[];
in vec2 tUv[];

out vec3 fragPos;
out vec4 fragPosLight;
out vec2 uv;

uniform mat4 lightMat;
uniform mat4 model, view, proj;
uniform sampler2D tex;

uniform sampler2D normTex;
uniform sampler2D grassPbrTex, stonePbrTex;

const float texScale = 0.1;
const float minRockH = -5.0;
const float maxRockH = 0.0;
const float grassBlend = 0.1;
const float dispStrength = 1.0;

vec3 getTriplanar(sampler2D tex, vec3 pos, vec3 norm) {
    vec3 blending = abs(norm);
    blending = normalize(max(blending, 0.00001));
    blending /= (blending.x + blending.y + blending.z);

    vec3 xaxis = texture(tex, pos.yz * texScale).rgb;
    vec3 yaxis = texture(tex, pos.xz * texScale).rgb;
    vec3 zaxis = texture(tex, pos.xy * texScale).rgb;

    return xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = mix(tPos[0], tPos[1], u);
    vec3 p1 = mix(tPos[3], tPos[2], u);
    vec3 pos = mix(p0, p1, v);

    vec2 uv0 = mix(tUv[0], tUv[1], u);
    vec2 uv1 = mix(tUv[3], tUv[2], u);
    uv = mix(uv0, uv1, v);

    pos.y = texture(tex, uv).r;
    fragPos = vec3(model * vec4(pos, 1.0));

    float heightPer = (fragPos.y - minRockH) / (maxRockH - minRockH);
    float heightFactor = clamp(heightPer, 0.0, 1.0);

    vec3 norm = normalize(texture(normTex, uv).xyz);
    float slope = 1 - dot(norm, vec3(0, 1, 0));
    float factor = clamp(slope * 1.5 + heightFactor * 2, 0.0, 1.0);
    float blend = smoothstep(0.5 - grassBlend, 0.5 + grassBlend, factor);

    float gDisp = getTriplanar(grassPbrTex, fragPos, norm).b;
    float sDisp = getTriplanar(stonePbrTex, fragPos, norm).b;
    float disp = mix(gDisp, sDisp, blend);

    pos += norm * (disp - 0.5) * dispStrength;
    fragPos = vec3(model * vec4(pos, 1.0));
    fragPosLight = lightMat * vec4(fragPos, 1.0);
    gl_Position = proj * view * vec4(fragPos, 1.0);
}
