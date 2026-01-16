#version 460 core
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D tex;
uniform bool useTex;

void main() {
    if (useTex) {
        float val = texture(tex, TexCoord).r + 35 / 70;
        // float val = texture(tex, TexCoord).r;
        FragColor = vec4(vec3(val), 1.0);
        // FragColor = vec4(vec3(texture(tex, TexCoord).r), 1.0);
        // FragColor = texture(tex, TexCoord);
    } else {
        FragColor = vec4(color, 1);
    }
}
