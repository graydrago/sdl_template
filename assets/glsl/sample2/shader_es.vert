#version 300 es
precision highp int;
precision highp float;

layout (location=0) in vec3 VertexPosition;

uniform vec3 CameraPosition;
uniform float PointSize;
uniform mat4 PVM;

void main() {
    gl_PointSize = PointSize/distance(CameraPosition, VertexPosition);
    /*gl_PointSize = 100.0;*/
    gl_Position = PVM * vec4(VertexPosition, 1.0);
}
