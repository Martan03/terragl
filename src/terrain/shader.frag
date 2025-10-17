#version 460 core
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    vec3 ambient = 0.2 * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    vec3 grassColor = vec3(0, 0.5, 0);
    vec3 rockColor  = vec3(0.15, 0.2, 0.2);

    float minHeight = 5.0;
    float maxHeight = 15.0;
    float heightFactor = clamp((fragPos.y - minHeight) / (maxHeight - minHeight), 0.0, 1.0);

    float slope = 1 - dot(norm, vec3(0, 1, 0));
    float factor = clamp(slope * 4 + heightFactor * 2, 0.0, 1.0);

    vec3 color = mix(grassColor, rockColor, factor);
    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1);
}
