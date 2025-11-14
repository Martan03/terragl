#version 460 core
layout(location = 0) in vec2 vertex;

uniform mat4 proj;

void main() {
    gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);
}
