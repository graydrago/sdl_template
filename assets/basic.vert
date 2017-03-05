#version 430

layout (location=0) in vec3 VertexPosition;

uniform vec3 Color;
uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(VertexPosition, 1.0);
}
