#version 460 core
in vec3 fragPos;
in vec4 fragPosLight;
in vec2 uv;

out vec4 FragColor;

uniform vec3 sunPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D normTex;

uniform sampler2D grassTex, grassNormTex, grassPbrTex;
uniform sampler2D stoneTex, stoneNormTex, stonePbrTex;

uniform sampler2D depthTex;

const float ambientStrength = 0.5;
const float specStrength = 0.04;

const float texScale = 0.1;
const float contrast = 1.0;

const float minGrassH = -25;
const float maxGrassH = -15;
const float minRockH = -5.0;
const float maxRockH = 0.0;
const float grassBlend = 0.1;

vec3 getBlending(vec3 norm) {
    vec3 blending = abs(norm);
    blending = normalize(max(blending, 0.00001));
    return blending / (blending.x + blending.y + blending.z);
}

vec3 getTriplanar(sampler2D tex, vec3 pos, vec3 norm) {
    vec3 blending = getBlending(norm);

    vec3 xaxis = texture(tex, pos.yz * texScale).rgb;
    vec3 yaxis = texture(tex, pos.xz * texScale).rgb;
    vec3 zaxis = texture(tex, pos.xy * texScale).rgb;

    return xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
}

vec3 getTriplanarNorm(sampler2D tex, vec3 pos, vec3 norm) {
    vec3 blending = getBlending(norm);

    vec3 xaxis = texture(tex, pos.yz * texScale).rgb * 2.0 - 1.0;
    vec3 yaxis = texture(tex, pos.xz * texScale).rgb * 2.0 - 1.0;
    vec3 zaxis = texture(tex, pos.xy * texScale).rgb * 2.0 - 1.0;

    vec3 wx = vec3(0.0, xaxis.y, xaxis.x) + vec3(norm.x, 0.0, 0.0);
    vec3 wy = vec3(yaxis.x, 0.0, yaxis.y) + vec3(0.0, norm.y, 0.0);
    vec3 wz = vec3(zaxis.x, zaxis.y, 0.0) + vec3(0.0, 0.0, norm.z);

    return normalize(wx * blending.x + wy * blending.y + wz * blending.z);
}

float shadowCalc(vec4 pos, vec3 norm) {
    vec3 projPos = pos.xyz / pos.w;
    projPos = projPos * 0.5 + 0.5;
    if (projPos.z > 1.0 || sunPos.y < 0)
        return 0.0;

    float bias = max(0.005 * (1.0 - dot(norm, sunPos)), 0.001);
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
    float heightPer = (fragPos.y - minRockH) / (maxRockH - minRockH);
    float heightFactor = clamp(heightPer, 0.0, 1.0);

    vec3 norm = normalize(texture(normTex, uv).xyz);
    float slope = 1 - dot(norm, vec3(0, 1, 0));
    float factor = clamp(slope * 1.5 + heightFactor * 2, 0.0, 1.0);
    float blend = smoothstep(0.5 - grassBlend, 0.5 + grassBlend, factor);

    vec3 gPbr = getTriplanar(grassPbrTex, fragPos, norm);
    vec3 sPbr = getTriplanar(stonePbrTex, fragPos, norm);
    float ao = mix(gPbr.r, sPbr.r, blend);
    float rough = mix(gPbr.g, sPbr.g, blend);

    vec3 gNorm = getTriplanarNorm(grassNormTex, fragPos, norm);
    vec3 sNorm = getTriplanarNorm(stoneNormTex, fragPos, norm);
    norm = normalize(mix(gNorm, sNorm, blend));

    vec3 grassColor = getTriplanar(grassTex, fragPos, norm);
    grassColor = (grassColor - 0.5) * 0.90 + 0.5;
    vec3 rockColor = getTriplanar(stoneTex, fragPos, norm);
    rockColor = (rockColor - 0.5) * contrast + 0.5;

    float lushFactor = smoothstep(minGrassH, maxGrassH, fragPos.y);
    vec3 lushColor = grassColor * vec3(0.9, 1.1, 0.9);
    vec3 dryColor = grassColor * vec3(0.95, 0.9, 0.9);
    grassColor = mix(lushColor, dryColor, lushFactor);
    vec3 color = mix(grassColor, rockColor, blend);

    vec3 lightDir = normalize(sunPos);
    float dayFact = sunPos.y >= 0 ? 1.0 : 0.0;
    float diff = dot(norm, lightDir);
    float shadow = 1.0;
    if (diff > 0.0) {
        shadow = shadowCalc(fragPosLight, norm);
    }
    vec3 diffuse = max(diff, 0.0) * (1.0 - shadow) * lightColor * dayFact;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float fres = pow(clamp(1.0 - dot(halfwayDir, viewDir), 0.0, 1.0), 5.0);
    fres *= specStrength + (1 - specStrength);

    float shininess = mix(128.0, 2.0, rough);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess) * dayFact;
    vec3 specular = spec * fres * (1.0 - rough) * lightColor * (1.0 - shadow);

    vec3 ambient = ambientStrength * lightColor * ao;

    vec3 result = (ambient + diffuse) * color + specular;
    FragColor = vec4(result, 1);
    // FragColor = vec4(pow(result, vec3(1.0 / 2.2)), 1.0);
}
