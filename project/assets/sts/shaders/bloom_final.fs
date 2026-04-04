[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec4 hdrColor = texture(scene, TexCoords);      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(bloom)
        hdrColor += vec4(bloomColor,0.0f); // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor.rgb * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}