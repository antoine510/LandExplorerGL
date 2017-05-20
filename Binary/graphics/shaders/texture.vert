#version 150 core

in vec2 in_Vertex;
in vec2 in_TexCoord0;

uniform mat3x2 transform;
uniform vec2 clipOffset;

out vec2 coordTexture;


void main()
{
    gl_Position = vec4(transform * vec3(in_Vertex, 1.0), 0.0, 1.0);

    coordTexture = in_TexCoord0 + clipOffset;
}
