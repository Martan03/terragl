#version 460 core
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D tex;

const float minRockH = 5.0;
const float maxRockH = 12.0;
const float grassBlend = 0;

void main() {
    vec3 ambient = 0.2 * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    vec3 grassColor = vec3(0, 0.5, 0);
    vec3 rockColor = vec3(0.15, 0.2, 0.2);

    float heightPer = (fragPos.y - minRockH) / (maxRockH - minRockH);
    float heightFactor = clamp(heightPer, 0.0, 1.0);

    float slope = 1 - dot(norm, vec3(0, 1, 0));
    float factor = clamp(slope * 8 + heightFactor * 2, 0.0, 1.0);
    float blend = smoothstep(0.5 - grassBlend, 0.5 + grassBlend, factor);

    vec3 color = mix(grassColor, rockColor, blend);
    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1);
    // FragColor = texture(tex, uv);
}
