#version 300 es
precision highp int;
precision highp float;

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

out vec4 ModelColor;

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform vec3 CameraPosition;
uniform Light light;
uniform Material material;
uniform mat4 VM;
uniform mat4 PVM;
uniform mat3 NM;

void main() {
    vec3 ambient = light.ambient * material.ambient;

    vec3 tnorm = normalize(NM * VertexNormal);
    vec3 eye = vec3(VM * vec4(VertexPosition, 1.0));
    vec3 s = normalize(vec3(light.position - eye));
    float sDotN = max(dot(s, tnorm), 0.0);
    vec3 diffuse = light.diffuse * material.diffuse * sDotN;

    vec3 r = reflect(-s, tnorm);
    vec3 v = normalize(CameraPosition - vec3(eye));
    vec3 specular =  vec3(0, 0, 0);
    if (sDotN > 0.0) {
      specular = light.specular * material.specular + pow(max(dot(r, v), 0.0), material.shininess);
    }

    ModelColor = vec4(ambient + diffuse + specular, 1);
    gl_Position = PVM * vec4(VertexPosition, 1);
}
