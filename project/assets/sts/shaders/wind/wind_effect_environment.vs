[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 MODELL;

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
    mat4 MODELL;
    float usePointLights[10];
} vs_out;

uniform mat4 PROJECTION;
uniform mat4 VIEW;
uniform mat4 PV;
uniform mat4 MODEL;
uniform mat4 lightSpaceMatrix;
uniform int numPointLights;
uniform PointLight pointLights[10];
uniform float TIME;
uniform float WINDEFFECT;

void main()
{
    float offset = sin(TIME) * (aPos.y) * WINDEFFECT;
    vs_out.MODELL = PV * MODELL;
    vs_out.FragPos = vec3(MODELL * vec4(aPos, 1.0)) + vec3(offset, 0.0, offset);
    vs_out.Normal = normalize(transpose(inverse(mat3(MODELL))) * aNormal);
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = PV * vec4(vs_out.FragPos, 1.0);

    for(int i = 0; i < numPointLights; i++)
    {
        vec3 lightDir = normalize(pointLights[i].position - vs_out.FragPos);

        float distance = length(pointLights[i].position - vs_out.FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));

        vs_out.usePointLights[i] = attenuation;
    }
}