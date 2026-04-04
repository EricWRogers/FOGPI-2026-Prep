[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    sampler2D specular;
    sampler2D diffuse;
    sampler2D emission;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec2 TEXELSHADOWSIZE;

uniform sampler2D shadowMap;

uniform mat4 PROJECTION;
uniform mat4 VIEW;
uniform mat4 PVM;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 COLOR;
uniform vec3 EMISSION;
uniform float EMISSIONUSINGALBEDOINTESITY;
uniform int numDirLights;
uniform DirLight dirLight;
uniform int numPointLights;
uniform PointLight pointLights[8];
uniform Material material;

uniform sampler2D SCREENTEXTURE;
uniform float TIME;
uniform vec2 distortion;

const float gamma = 2.2;

vec3 emission;

vec3 lerp(vec3 _min, vec3 _max, float _fraction);

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{           
    // properties
    emission = texture(material.emission, fs_in.TexCoords).rgb;
    vec3 norm = fs_in.Normal;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 result = vec3(texture(material.diffuse, fs_in.TexCoords) * COLOR);

    // alpha
    float alpha = min(COLOR.a, texture(material.diffuse, fs_in.TexCoords).a);

    //vec3 result = pow(dirResult, vec3(1.0 / gamma));

    if (alpha >= 1.0)
        BrightColor = vec4(0.0, 0.0, 0.0, 0.0);

    // check whether result is higher than some threshold, if so, output as bloom threshold COLOR
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        BrightColor = vec4(result+0.3, alpha);
    }
    else {
        BrightColor = vec4(0.0, 0.0, 0.0, alpha);
    }

    if (emission.r > 0.0)
    {
        BrightColor = vec4(EMISSION * emission, 1.0);
    }

    if (EMISSIONUSINGALBEDOINTESITY > 0.0) {
        BrightColor = vec4(EMISSIONUSINGALBEDOINTESITY * result * emission, 1.0);
    }

    FragColor = vec4(result, alpha);
}

vec3 lerp(vec3 _min, vec3 _max, float _fraction)
{
    return _min + _fraction * (_max - _min);
}