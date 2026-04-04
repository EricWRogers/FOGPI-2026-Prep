[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 PROJECTION;
uniform mat4 VIEW;
uniform mat4 MODEL;
uniform mat4 PVM;
uniform mat4 lightSpaceMatrix;

out vec3 centerPos;

void main()
{
    vs_out.FragPos = vec3(MODEL * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(MODEL))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = PVM * vec4(aPos, 1.0);
    centerPos = vec3(MODEL * vec4(0.0, 0.0, 0.0, 1.0));
}