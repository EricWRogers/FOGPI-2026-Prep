[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aModelInstance;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform float TIME;
uniform float WINDEFFECT;

void main()
{
    float offset = sin(TIME) * (aPos.y) * WINDEFFECT;
    vec3 position = vec3(aModelInstance * vec4(aPos, 1.0)) + vec3(offset, 0.0, offset);
    gl_Position = lightSpaceMatrix * vec4(position, 1.0);
}