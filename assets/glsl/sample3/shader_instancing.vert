#version 300 es
precision mediump int;
precision mediump float;

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aInstancePos;

out vec3 vColor;

uniform mat4 uPV;

void main() {
    vColor = aColor;
    gl_Position = uPV * vec4(aVertexPosition + aInstancePos, 0.0, 1.0);
}
