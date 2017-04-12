precision mediump float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;

uniform mat4 VM;
uniform mat4 PVM;

varying vec3 Color;

void main() {
    Color = vec3(1.0, 0.0, 0.0);
    gl_Position = PVM * vec4(VertexPosition, 1.0);
}
