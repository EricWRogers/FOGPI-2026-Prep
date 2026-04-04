[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform float TIME;

uniform float OFFSET;
uniform float EFFECT;
uniform float HEIGHTMULTIPLIER;

void main()
{
    float offset = 0.0;
    float effect = EFFECT*0.25;

    if (TIME*5.0 < TIME*4.9)
        effect = EFFECT;

    if (OFFSET < abs(aPos.x))
        offset = sin(TIME*5.0) * abs(aPos.x) * effect;
    
    float birdOffset = (sin(TIME*5.0) * effect - 0.5) * HEIGHTMULTIPLIER;

    float dir = (aPos.x > 0) ? -1 : 1;
    
    vec3 position = vec3(model * vec4(aPos + vec3(dir * abs(offset*0.5), offset + birdOffset, 0.0), 1.0));
    gl_Position = lightSpaceMatrix * vec4(position, 1.0);
}