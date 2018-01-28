#version 300 es
precision highp int;
precision highp float;

out vec4 FragColor;

uniform vec3 Color;
uniform sampler2D tex;

void main () {
    vec4 t = texture(tex, gl_PointCoord);
    if (t.a < 0.0001) {
      discard;
    }
    FragColor = t;
}
