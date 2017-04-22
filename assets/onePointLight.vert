#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

out vec3 ModelColor;

vec3 LightPosition = vec3(0.0, 10.0, 10.0);

uniform vec3 Color;
uniform mat4 VM;
uniform mat4 PVM;
uniform mat3 NM;

void main() {
    vec3 tnorm = normalize(NM * VertexNormal);
    vec4 eye = VM * vec4(VertexPosition, 1.0);
    vec3 s = normalize(vec3(LightPosition - vec3(eye)));
    ModelColor = Color * max(dot(s, tnorm), 0.0);

    gl_Position = PVM * vec4(VertexPosition, 1.0);
}
