#version 300 es
precision highp int;
precision highp float;

in int vPlanetID;

out vec4 fFragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;

void main () {
    vec4 t;
    if (vPlanetID == 0) {
        vec4 t = texture(uTex0, gl_PointCoord);
    } else {
        vec4 t = texture(uTex1, gl_PointCoord);
    }
    
    if (t.a < 0.0001) {
      discard;
    }
    fFragColor = t;
}
