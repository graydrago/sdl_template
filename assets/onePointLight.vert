#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

out vec3 Color;

/*uniform vec4 LightPosition;*/
vec3 LightPosition = vec3(0.0, 2.0, 0.0);
vec3 ObjectColor = vec3(1.0, 0.0, 0.0);

uniform mat4 VM = mat4(1.0);
uniform mat4 PVM = mat4(1.0);

void main() {
    vec4 vertex = PVM * vec4(VertexPosition, 1.0);
    vec4 normal = PVM * vec4(VertexNormal, 1.0);
    vec4 eyeCoord = VM * vec4(VertexPosition, 1.0);
    vec4 d = vec4(LightPosition, 1.0) - eyeCoord;

    Color = ObjectColor * dot(d, normal);

    gl_Position = vertex;
    /*gl_Position = PVM * vec4(VertexPosition, 1.0);*/
}
