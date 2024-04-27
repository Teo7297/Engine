#version 330 core
out vec4 color;

in VS_OUT{
    vec2 TexCoords;
    flat int index;
}fs_in;

uniform sampler2DArray text;
uniform int letterMap[250];
uniform vec3 textColor;

uniform float thickness = 0.55f;
uniform float outline_thickness = 0.51f;
uniform float softness = 0.035f;
uniform float outline_softness = 0.065f;
uniform vec4 outline_color = {0,0,0,1};

void main()
{    
    float glyphShape = texture(text, vec3(fs_in.TexCoords.xy,letterMap[fs_in.index])).r;
    // color = vec4(textColor, glyphShape);
    float outline = smoothstep(outline_thickness - outline_softness, outline_thickness + outline_softness, glyphShape);
    glyphShape = smoothstep(1.0 - thickness - softness, 1.0 - thickness + softness, glyphShape);
    
    color = vec4(mix(outline_color.rgb, textColor.rgb, outline), glyphShape);
}