#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;

uniform float Scale = 1.0;
uniform mat4 Rotation = mat4(1.0);

out vec3 Color;

void main() {
    Color = VertexColor;
    gl_Position = Rotation * vec4(VertexPosition * Scale, 1.0);
}
