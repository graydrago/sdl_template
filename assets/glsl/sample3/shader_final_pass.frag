#version 300 es
precision mediump float;

#define RANGE 2
#define SAMPLES 25

out vec4 fFragColor;

uniform sampler2D uScreen;
uniform vec3 uScreenSize;
uniform float uOffset;

void main () {
  vec4 samples[SAMPLES];
  int id = 0;
  for (int row = -RANGE; row <= RANGE; ++row) {
    for (int col = -RANGE; col <= RANGE; ++col) {
      samples[id] = texture(uScreen,
                      vec2(float(gl_FragCoord.x + float(col)*uOffset)/uScreenSize.x,
                           float(gl_FragCoord.y + float(row)*uOffset)/uScreenSize.y));
      id++;
    }
  }

  /*float kernel[9] = float[](*/
      /*1.0/16.0, 2.0/16.0, 1.0/16.0,*/
      /*2.0/16.0, 4.0/16.0, 2.0/16.0,*/
      /*1.0/16.0, 2.0/16.0, 1.0/16.0*/
  /*);*/

  vec4 c = vec4(0.0, 0.0, 0.0, 1.0);
  for (int i = 0; i < SAMPLES; ++i) {
    /*c += samples[i] * kernel[i];*/
    c += samples[i];
  }

  c /= float(SAMPLES) * 0.5; // 0.5 - make brighter

  fFragColor = c;
}
