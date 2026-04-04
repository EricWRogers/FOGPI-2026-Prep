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

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec2 FragUV;
} fs_in;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 COLOR;
uniform vec3 EMISSION;
uniform float EMISSIONUSINGALBEDOINTESITY;
uniform int numDirLights;
uniform DirLight dirLight;
uniform Material material;
uniform float NEARPLANE;
uniform float FARPLANE;

uniform sampler2D NOISE;
uniform sampler2D SCREENTEXTURE;
uniform sampler2D DEPTHTEXTURE;
uniform float TIME;

const vec2 distortion = vec2(0.002);
const float gamma = 2.2;

vec3 emission;

vec3 lerp(vec3 _min, vec3 _max, float _fraction);
vec4 lerp(vec4 _min, vec4 _max, float _fraction);
float ShadowCalculation(vec4 fragPosLightSpace);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{           
    // properties
    emission = texture(material.emission, fs_in.TexCoords).rgb;
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    float depth = texture(DEPTHTEXTURE, fs_in.FragUV.xy).r;
    float floorDistance = 2.0 * NEARPLANE * FARPLANE / (FARPLANE + NEARPLANE - (2.0 * depth - 1.0) * (FARPLANE - NEARPLANE));
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * NEARPLANE * FARPLANE / (FARPLANE + NEARPLANE - (2.0 * depth - 1.0) * (FARPLANE - NEARPLANE));
    float waterDepth = floorDistance - waterDistance;
    float maxVisableDepth = 2.5;
    float seaFoamDepth = 0.2;

    //if (waterDistance > floorDistance)
    //    discard;

    vec3 dirResult;
    if (numDirLights > 0)
        dirResult = CalcDirLight(dirLight, norm, viewDir);

    // alpha
    float alpha = min(COLOR.a, texture(material.diffuse, fs_in.TexCoords).a);

    vec3 result = dirResult;
    //vec3 result = pow(dirResult, vec3(1.0 / gamma));

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
        if (EMISSIONUSINGALBEDOINTESITY > 0.0) {
            BrightColor = vec4(EMISSIONUSINGALBEDOINTESITY * result * emission, 1.0);
        } else {
            BrightColor = vec4(EMISSION * emission, 1.0);
        }
    }

    // Calculate noise offsets based on UV and TIME
    //float noiseY = rand(vec2(0.0, TIME * 0.001));
    //float noiseX = rand(vec2(TIME * 0.001, noiseY));

    // Calculate distortion offsets using noise
    //vec2 distortedTexCoord = vec2(noiseX, noiseY) * 2.0 - 1.0;
    //distortedTexCoord *= distortion;
    vec2 time = vec2(TIME, TIME);
    vec2 noiseOffset = (
        texture(NOISE, fs_in.FragUV.xy + (time * vec2(0.34,0.12))).xy +
        texture(NOISE, fs_in.FragUV.xy + (time * vec2(0.13,0.26))).xz)
         * vec2(0.001, 0.001);
    vec4 screenColor = texture(SCREENTEXTURE, fs_in.FragUV.xy + (noiseOffset));// + distortedTexCoord);
    screenColor *= 1 - alpha;
    result *= alpha;
    float whiteCap = noiseOffset.y * 300.0;
    whiteCap += result.b;

    if ((whiteCap + result.b) < 1.4)
        whiteCap = 0.0;
    
    if ((whiteCap + result.b) > 1.0)
        whiteCap = (1.0 - result.b);
    
    if (waterDistance > floorDistance)
    {
        FragColor = lerp(
            screenColor + vec4(result, 1.0),
            vec4(result, 1.0),
            clamp(waterDepth/maxVisableDepth, 0.0, 1.0)
        );
        return;
    }

    FragColor = lerp(
                    vec4(1.0),
                    lerp(
                        screenColor + vec4(result + whiteCap, 1.0),
                        vec4(result + whiteCap, 1.0),
                        clamp(waterDepth/maxVisableDepth, 0.0, 1.0)
                    ),
                    clamp(waterDepth/(seaFoamDepth), 0.0, 1.0)
                );
}

vec3 lerp(vec3 _min, vec3 _max, float _fraction)
{
    return _min + _fraction * (_max - _min);
}

vec4 lerp(vec4 _min, vec4 _max, float _fraction)
{
    return _min + _fraction * (_max - _min);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        return 0.0;
    
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.002);
    //float bias = 0.001;
    // check whether current frag pos is in shadow
    //return currentDepth - 0.0000000001 > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float pcfDepth = 0.0;
    /*for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }*/

    pcfDepth = texture(shadowMap, projCoords.xy + vec2(-1, -1) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 
    pcfDepth = texture(shadowMap, projCoords.xy + vec2(-1, 0) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 
    pcfDepth = texture(shadowMap, projCoords.xy + vec2(-1, 1) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 

    pcfDepth = texture(shadowMap, projCoords.xy + vec2(0, -1) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
    pcfDepth = texture(shadowMap, projCoords.xy + vec2(0, 1) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 

    pcfDepth = texture(shadowMap, projCoords.xy + vec2(1, -1) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 
    pcfDepth = texture(shadowMap, projCoords.xy + vec2(1, 0) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 
    pcfDepth = texture(shadowMap, projCoords.xy + vec2(1, 1) * texelSize).r; 
    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; 
        
    return shadow/9.0;
}

// calculates the COLOR when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 baseColor = (vec3(COLOR) * vec3(texture(material.diffuse, fs_in.TexCoords)));
    vec3 ambient = light.ambient * baseColor;
    vec3 diffuse = light.diffuse * diff * baseColor;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace) - length(emission);
    return (ambient + (1.0 - max(0.0, shadow*0.75)) * (diffuse + specular));
}