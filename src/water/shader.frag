#version 460 core
in float depth;
in vec3 fragPos;

out vec4 FragColor;

uniform float amp;
uniform vec3 sunPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

const float ambientStrength = 0.2;
const float specStrength = 0.5;

// float shadowCalc(vec4 pos, vec3 norm) {
//     vec3 projPos = pos.xyz / pos.w;
//     projPos = projPos * 0.5 + 0.5;
//     if (projPos.z > 1.0 || sunPos.y < 0)
//         return 0.0;

//     float bias = max(0.005 * (1.0 - dot(norm, sunPos)), 0.001);
//     vec2 tsize = 1.0 / textureSize(depthTex, 0);

//     float shadow = 0.0;
//     for (int x = -1; x <= 1; ++x) {
//         for (int y = -1; y <= 1; ++y) {
//             float pcf = texture(depthTex, projPos.xy + vec2(x, y) * tsize).r;
//             shadow += (projPos.z - bias > pcf) ? 1.0 : 0.0;
//         }
//     }
//     return shadow / 9.0;
// }

void main() {
    if (depth <= 0) discard;

    vec3 ambient = ambientStrength * lightColor;
    float alpha = clamp(depth * 2.0, 0.4, 0.8);

    vec3 lightDir = normalize(sunPos);
    float diff = dot(vec3(0, 1, 0), lightDir);
    float shadow = 0.0;
    vec3 diffuse = max(diff, 0.0) * (1.0 - shadow) * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-sunPos, vec3(0, 1, 0));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    diffuse += specStrength * spec * lightColor * (1.0 - shadow);

    vec3 deepColor = vec3(0.02, 0.1, 0.2);
    vec3 shallowColor = vec3(0.1, 0.5, 0.6);
    float depthFact = clamp(depth / 5.0, 0.0, 1.0);
    vec3 baseColor = mix(shallowColor, deepColor, depthFact);

    vec3 result = (ambient + diffuse) * baseColor;
    FragColor = vec4(result, alpha);
}
