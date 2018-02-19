#version 300 es
precision mediump int;
precision mediump float;

out vec4 fFragColor;

in vec3 vColor;

void main () {
    fFragColor = vec4(vColor, 1.0);
}
