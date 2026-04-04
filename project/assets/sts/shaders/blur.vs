[OPENGL VERSION]

#ifdef GL_ES
    precision mediump float;
#endif

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 tex_offset;

uniform sampler2D image;

void main()
{
    tex_offset = 1.0 / textureSize(image, 0);
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}