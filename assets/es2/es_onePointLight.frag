precision mediump float;

varying vec3 ModelColor;

void main () {
    gl_FragColor = vec4(ModelColor, 1.0);
}
