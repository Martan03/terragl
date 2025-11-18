#version 460 core
layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 proj;

void main() {
    gl_Position = proj * model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
