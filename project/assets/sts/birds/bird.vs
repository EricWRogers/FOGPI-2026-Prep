[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    mat4 MODEL;
    float usePointLights[10];
} vs_out;

uniform mat4 PROJECTION;
uniform mat4 VIEW;
uniform mat4 PVM;
uniform mat4 MODEL;
uniform mat4 lightSpaceMatrix;
uniform int numPointLights;
uniform PointLight pointLights[10];
uniform float TIME;

uniform float OFFSET;
uniform float EFFECT;
uniform float HEIGHTMULTIPLIER;
uniform float SPEED;
uniform float WINGOFFSET;


void main()
{
    float offset = 0.0;
    float effect = EFFECT*0.25;

    // what are the following lines
    if (TIME*SPEED < TIME*(SPEED-0.1))
        effect = EFFECT;

    if (OFFSET < abs(aPos.x))
        offset = sin(TIME*SPEED+WINGOFFSET) * abs(aPos.x) * effect;
    
    float birdOffset = (sin(TIME*SPEED+WINGOFFSET) * effect - 0.5) * HEIGHTMULTIPLIER;

    float dir = (aPos.x > 0) ? -1 : 1;
    
    vs_out.FragPos = vec3(MODEL * vec4(aPos + vec3(dir * abs(offset*0.5), offset + birdOffset, 0.0), 1.0));
    vs_out.Normal = normalize(transpose(inverse(mat3(MODEL))) * aNormal);
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = PVM * vec4(aPos + vec3(dir * abs(offset*0.5), offset + birdOffset, 0.0), 1.0);

    for(int i = 0; i < numPointLights; i++)
    {
        vec3 lightDir = normalize(pointLights[i].position - vs_out.FragPos);

        float distance = length(pointLights[i].position - vs_out.FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));

        vs_out.usePointLights[i] = attenuation;
    }
}