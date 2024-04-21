#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D textureSlot;
uniform bool textureAvailable;

void main()
{
    if(textureAvailable)
        FragColor = texture(textureSlot, TexCoord);
    else
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 