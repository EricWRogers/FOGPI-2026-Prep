[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

out vec4 FragColor;

in vec2 TexCoords;
in vec2 tex_offset;

uniform sampler2D image;
uniform bool horizontal;
uniform float weight[8] = float[] (0.22, 0.19, 0.12, 0.0540540541, 0.0162162162
                                    , 0.0142162162, 0.0122162162, 0.0102162162);

float scaleOffset = 2.0;

void main()
{             
    vec2 tex_scale;
    vec4 baseImage = texture(image, TexCoords);
    vec3 result = baseImage.rgb * weight[0];

    if(horizontal) {
        for(int i = 1; i < 3; ++i) {
            tex_scale = vec2(tex_offset.x * i * scaleOffset, 0.0);
            result += texture(image, TexCoords + tex_scale).rgb * weight[i];
            result += texture(image, TexCoords - tex_scale).rgb * weight[i];
        }
    } else {
        for(int i = 1; i < 3; ++i) {
            tex_scale = vec2(0.0, tex_offset.y * i * scaleOffset);
            result += texture(image, TexCoords + tex_scale).rgb * weight[i];
            result += texture(image, TexCoords - tex_scale).rgb * weight[i];
        }
    }

    if (baseImage.r + baseImage.g + baseImage.b > result.r + result.g + result.b)
        FragColor = baseImage;
    else
        FragColor = vec4(result, baseImage.a);
}