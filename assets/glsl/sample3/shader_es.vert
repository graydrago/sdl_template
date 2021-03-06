#version 300 es
precision mediump int;
precision mediump float;

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec3 aColor;

out vec3 vColor;

uniform mat4 uPVL;

void main() {
    vColor = aColor;
    gl_Position = uPVL * vec4(aVertexPosition, 0.0, 1.0);
}
