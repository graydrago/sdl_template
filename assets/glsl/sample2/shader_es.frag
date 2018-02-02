#version 300 es
precision highp int;
precision highp float;

flat in int vPlanetID;

out vec4 fFragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform sampler2D uTex2;
uniform sampler2D uTex3;
uniform sampler2D uTex4;
uniform sampler2D uTex5;
uniform sampler2D uTex6;
uniform sampler2D uTex7;
uniform sampler2D uTex8;
uniform sampler2D uTex9;
uniform sampler2D uTex10;
uniform sampler2D uTex11;
uniform sampler2D uTex12;
uniform sampler2D uTex13;
uniform sampler2D uTex14;
uniform sampler2D uTex15;
uniform sampler2D uTex16;
uniform sampler2D uTex17;

void main () {
    vec4 t;
    if (vPlanetID == 0) {
        t = texture(uTex0, gl_PointCoord);
    } else if (vPlanetID == 1) {
        t = texture(uTex1, gl_PointCoord);
    } else if (vPlanetID == 2) {
        t = texture(uTex2, gl_PointCoord);
    } else if (vPlanetID == 3) {
        t = texture(uTex3, gl_PointCoord);
    } else if (vPlanetID == 4) {
        t = texture(uTex4, gl_PointCoord);
    } else if (vPlanetID == 5) {
        t = texture(uTex5, gl_PointCoord);
    } else if (vPlanetID == 6) {
        t = texture(uTex6, gl_PointCoord);
    } else if (vPlanetID == 7) {
        t = texture(uTex7, gl_PointCoord);
    } else if (vPlanetID == 8) {
        t = texture(uTex8, gl_PointCoord);
    } else if (vPlanetID == 9) {
        t = texture(uTex9, gl_PointCoord);
    } else if (vPlanetID == 10) {
        t = texture(uTex10, gl_PointCoord);
    } else if (vPlanetID == 11) {
        t = texture(uTex11, gl_PointCoord);
    } else if (vPlanetID == 12) {
        t = texture(uTex12, gl_PointCoord);
    } else if (vPlanetID == 13) {
        t = texture(uTex13, gl_PointCoord);
    } else if (vPlanetID == 14) {
        t = texture(uTex14, gl_PointCoord);
    } else if (vPlanetID == 15) {
        t = texture(uTex15, gl_PointCoord);
    } else if (vPlanetID == 16) {
        t = texture(uTex16, gl_PointCoord);
    } else {
        t = texture(uTex17, gl_PointCoord);
    }
    
    if (t.a < 0.0001) {
      discard;
    }
    fFragColor = t;
}
