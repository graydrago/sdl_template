#version 300 es
precision highp int;
precision highp float;

layout (location=0) in vec3 aVertexPosition;

out int vPlanetID;

uniform vec3 uCameraPosition;
uniform float uPointSize;
uniform mat4 uPVM;

void main() {
    gl_PointSize = uPointSize/distance(uCameraPosition, aVertexPosition);

    if (gl_VertexID > 5000) {
      vPlanetID = 0;
    } else {
      vPlanetID = 1;
    }

    gl_Position = PVM * vec4(uVertexPosition, 1.0);
}
