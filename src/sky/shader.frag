#version 460 core
in vec3 texCoord;

out vec4 FragColor;

uniform vec3 sunPos;
// const vec3 sunPos = vec3(0, 1, 0);
const float sunIntensity = 60.0;

const float PI = 3.14159;
// Earth radius and atmosphere radius
const float Re = 6371e3;
const float Ra = 6420e3;
// Rayleigh and mie scale height
const float Hr = 8e3;
const float Hm = 1.2e3;

// Scatter coefficients (blue/violet dominant and white/grey)
const vec3 betaR = vec3(5.5e-6, 13.0e-6, 22.4e-6);
const vec3 betaM = vec3(21e-6);

vec2 intersectSphere(vec3 origin, vec3 dir, float radius) {
    float b = dot(origin, dir);
    float d = b * b - dot(origin, origin) + radius * radius;
    if (d < 0.0) return vec2(-1.0);
    float sqrtD = sqrt(d);
    return vec2(-b - sqrtD, -b + sqrtD);
}

void main() {
    vec3 viewDir = normalize(texCoord);
    // Lower horizon so that it's below terrain on spawn position
    viewDir.y += 0.4;
    viewDir = normalize(viewDir);

    vec3 camPos = vec3(0, Re + 1.0, 0);
    vec2 hits = intersectSphere(camPos, viewDir, Ra);
    if (hits.y < 0.0) {
        FragColor = vec4(0, 0, 0, 1);
        return;
    }

    vec3 totalR = vec3(0.0), totalM = vec3(0.0);
    float opticalDepthR = 0.0, opticalDepthM = 0.0;

    int steps = 16;
    float stepSize = hits.y / float(steps);
    for (int i = 0; i < steps; ++i) {
        vec3 p = camPos + viewDir * (stepSize * 0.5);
        float height = length(p) - Re;

        float hr = exp(-height / Hr) * stepSize;
        float hm = exp(-height / Hm) * stepSize;
        opticalDepthR += hr;
        opticalDepthM += hm;

        vec2 sunHits = intersectSphere(p, sunPos, Ra);
        float opticalDepthLightR = 0.0, opticalDepthLightM = 0.0;

        float stepSizeLight = sunHits.y / float(8);
        for (int j = 0; j < 8; ++j) {
            vec3 pLight = p + sunPos * (stepSizeLight * (float(j) + 0.5));
            float heightLight = length(pLight) - Re;
            if (heightLight < 0.0) {
                opticalDepthLightR = 1e20;
                break;
            }
            opticalDepthLightR += exp(-heightLight / Hr) * stepSizeLight;
            opticalDepthLightM += exp(-heightLight / Hm) * stepSizeLight;
        }

        vec3 tau = betaR * (opticalDepthR + opticalDepthLightR) +
                betaM * 1.1 * (opticalDepthM + opticalDepthLightM);
        vec3 attenuation = exp(-tau);

        totalR += hr * attenuation;
        totalM += hm * attenuation;

        camPos += viewDir * stepSize;
    }

    float mu = dot(viewDir, sunPos);
    float mu2 = mu * mu;

    float phaseR = 3.0 / (16.0 * PI) * (1.0 + mu2);

    float g = 0.99;
    float phaseM = 3.0 / (8.0 * PI) * ((1.0 - g * g) * (1.0 + mu2))
            / pow(1.0 + g * g - 2.0 * g * mu, 1.5);

    vec3 color = totalR * betaR * phaseR + totalM * betaM * phaseM;
    FragColor = vec4(1.0 - exp(-color * sunIntensity), 1.0);
}
