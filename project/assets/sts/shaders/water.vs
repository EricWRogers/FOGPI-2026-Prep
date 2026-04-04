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
    vec2 FragUV;
} vs_out;

uniform mat4 PROJECTION;
uniform mat4 VIEW;
uniform mat4 MODEL;
uniform mat4 PVM;
uniform mat4 lightSpaceMatrix;

uniform sampler2D NOISE;
uniform sampler2D SCREENTEXTURE;
uniform float TIME;

void main()
{
    // could be better
    vec4 clipPosition = PVM * vec4(aPos, 1.0);
    clipPosition /= clipPosition.w;

    vec2 fraguv = (clipPosition.xy + vec2(1.0)) * 0.5;

    vec2 time = vec2(TIME, TIME);

    vec2 noiseOffset = ( // scroll speed = (time * vec2(0.15,0.07))
        texture(NOISE, fraguv + (time * vec2(0.15,0.07))).xy +
        texture(NOISE, fraguv + (time * vec2(0.07,0.15))).xz)
         * vec2(0.4, 0.4);
    
    vec3 position = aPos + (aNormal * vec3(0.0, (noiseOffset.y * 1.0) - 0.5, 0.0));

    vs_out.FragPos = vec3(MODEL * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(MODEL))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = PVM * vec4(position, 1.0);

    clipPosition = gl_Position;
    clipPosition /= clipPosition.w;

    vs_out.FragUV = (clipPosition.xy + vec2(1.0)) * 0.5;
}