[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aModelInstance;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform float TIME;

void main()
{
    gl_Position = lightSpaceMatrix * aModelInstance * vec4(aPos, 1.0);
}