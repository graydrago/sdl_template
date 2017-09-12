#version 300 es
precision highp int;
precision highp float;

out highp vec4 FragColor;

in highp vec4 ModelColor;

uniform vec3 CameraPosition;
uniform vec3 LightPosition;
uniform vec3 Color;

float specularStrength = 0.5;

void main () {
    FragColor = ModelColor;
}
