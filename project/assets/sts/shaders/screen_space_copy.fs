[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

void main()
{             
    FragColor = texture(image, TexCoords);
}