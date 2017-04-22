#version 430

out vec4 FragColor;

in vec3 ModelColor;

void main () {
    FragColor = vec4(ModelColor, 1.0);
}
