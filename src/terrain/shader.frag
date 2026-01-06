#version 460 core
in vec3 fragPos;
in vec4 fragPosLight;
in vec2 uv;

out vec4 FragColor;

uniform vec3 sunPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D normTex;

uniform sampler2D grassTex;
uniform sampler2D stoneTex;

uniform sampler2D depthTex;

const float texScale = 0.1;
const float contrast = 1.2;

const float minGrassH = -25;
const float maxGrassH = -15;
const float minRockH = -5.0;
const float maxRockH = 0.0;
const float grassBlend = 0.1;

vec3 getTriplanar(sampler2D tex, vec3 pos, vec3 norm) {
    vec3 blending = abs(norm);
    blending = normalize(max(blending, 0.00001));
    float b = (blending.x + blending.y + blending.z);
    blending /= b;

    vec3 xaxis = texture(tex, pos.yz * texScale).rgb;
    vec3 yaxis = texture(tex, pos.xz * texScale).rgb;
    vec3 zaxis = texture(tex, pos.xy * texScale).rgb;

    vec3 res = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
    return (res - 0.5) * contrast + 0.5;
}

float shadowCalc(vec4 pos, vec3 norm) {
    vec3 projPos = pos.xyz / pos.w;
    projPos = projPos * 0.5 + 0.5;
    if (projPos.z > 1.0 || sunPos.y < 0)
        return 0.0;

    float bias = max(0.005 * (1.0 - dot(norm, sunPos)), 0.001);
    // bias = 0.001;
    vec2 tsize = 1.0 / textureSize(depthTex, 0);

    float shadow = 0.0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcf = texture(depthTex, projPos.xy + vec2(x, y) * tsize).r;
            shadow += (projPos.z - bias > pcf) ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}

void main() {
    vec3 ambient = 0.2 * lightColor;

    vec3 norm = normalize(texture(normTex, uv).xyz);

    vec3 lightDir = normalize(sunPos);
    // vec3 lightDir = normalize(lightPos - fragPos);
    float diff = dot(norm, lightDir);
    float shadow = 1.0;
    if (diff > 0.0) {
        shadow = shadowCalc(fragPosLight, norm);
    }
    vec3 diffuse = max(diff, 0.0) * (1.0 - shadow) * lightColor;

    vec3 grassColor = getTriplanar(grassTex, fragPos, norm);
    vec3 rockColor = getTriplanar(stoneTex, fragPos, norm);

    float lushFactor = smoothstep(minGrassH, maxGrassH, fragPos.y);
    vec3 lushColor = grassColor * vec3(0.9, 1.1, 0.9);
    vec3 dryColor = grassColor * vec3(0.95, 0.9, 0.9);
    grassColor = mix(lushColor, dryColor, lushFactor);

    float heightPer = (fragPos.y - minRockH) / (maxRockH - minRockH);
    float heightFactor = clamp(heightPer, 0.0, 1.0);

    float slope = 1 - dot(norm, vec3(0, 1, 0));
    float factor = clamp(slope * 8 + heightFactor * 2, 0.0, 1.0);
    float blend = smoothstep(0.5 - grassBlend, 0.5 + grassBlend, factor);

    vec3 color = mix(grassColor, rockColor, blend);
    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1);
}
