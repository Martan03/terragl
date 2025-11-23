#version 460 core
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D tex;
uniform bool useTex;

void main() {
    if (useTex) {
        FragColor = texture(tex, TexCoord);
    } else {
        FragColor = vec4(color, 1);
    }
}
