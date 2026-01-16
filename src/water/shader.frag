#version 460 core
in float depth;
in float waterHeight;
in vec3 fragPos;

out vec4 FragColor;

uniform float amp;
uniform vec3 sunPos;

void main() {
    // if (waterHeight <= -amp + 0.1 || depth <= 0.05) discard;

    float alpha = clamp(depth * 2.0, 0.0, 0.8);

    vec3 deepColor = vec3(0.02, 0.1, 0.2);
    vec3 shallowColor = vec3(0.1, 0.5, 0.6);
    float depthFact = clamp(depth / 5.0, 0.0, 1.0);
    vec3 baseColor = mix(shallowColor, deepColor, depthFact);

    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(sunPos + viewDir);
    float spec = pow(max(dot(vec3(0, 1, 0), halfwayDir), 0.0), 64.0);

    vec3 result = baseColor + (spec * vec3(1.0, 0.9, 0.7));
    FragColor = vec4(result, alpha);
}
