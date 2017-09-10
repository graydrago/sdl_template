#version 300 es
precision highp int;
precision highp float;

out highp vec4 FragColor;

in highp vec4 ModelColor;

void main () {
    FragColor = ModelColor;
}
