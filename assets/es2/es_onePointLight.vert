precision mediump float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;

vec3 LightPosition = vec3(0.0, 10.0, 10.0);
vec3 ObjectColor = vec3(1.0, 0.0, 0.0);

uniform mat4 VM;
uniform mat4 PVM;
uniform mat3 NM;

varying vec3 Color;

void main() {
    vec3 tnorm = normalize(NM * VertexNormal);
    vec4 eye = VM * vec4(VertexPosition, 1.0);
    vec3 s = normalize(vec3(LightPosition - vec3(eye)));
    Color = ObjectColor * max(dot(s, tnorm), 0.0);

    gl_Position = PVM * vec4(VertexPosition, 1.0);
}
