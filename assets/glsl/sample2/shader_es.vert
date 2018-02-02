#version 300 es
precision highp int;
precision highp float;

layout (location=0) in vec3 aVertexPosition;

flat out int vPlanetID;

uniform vec3 uCameraPosition;
uniform float uPointSize;
uniform mat4 uPVM;
uniform float uTime;
uniform int uMaxTextureUnits;

void main() {
    float r = length(aVertexPosition.xz);
    float a = atan(aVertexPosition.z, aVertexPosition.x) + uTime * (1.0-r/300.0) * 0.1;
    vec3 point = vec3(r*cos(a), aVertexPosition.y, r*sin(a));

    gl_PointSize = uPointSize/distance(uCameraPosition, point);

    vPlanetID = gl_VertexID % uMaxTextureUnits;
    gl_Position = uPVM * vec4(point, 1.0);
}
