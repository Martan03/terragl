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

    vec3 result = (ambient + diffuse) * vec3(0, 0.5, 0);
    FragColor = vec4(result, 1);
}
