precision mediump float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec3 VertexColor;

uniform vec3 LightPosition;
uniform mat4 VM;
uniform mat4 PVM;
uniform mat3 NM;

varying vec3 ModelColor;

void main() {
    vec3 tnorm = normalize(NM * VertexNormal);
    vec4 eye = VM * vec4(VertexPosition, 1.0);
    vec3 s = normalize(vec3(LightPosition - vec3(eye)));

    ModelColor = VertexColor * max(dot(s, tnorm), 0.0);
    gl_Position = PVM * vec4(VertexPosition, 1.0);
}
