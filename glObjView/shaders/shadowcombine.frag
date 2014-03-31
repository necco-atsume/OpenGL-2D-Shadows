#version 150 compatibility

uniform sampler2D uIntensity;
uniform sampler2D uMask;

varying vec2 vST;

out vec4 Color;

void main() {
    //FIXME: Ambient component should be set as a uniform.
    
    Color = ((1 - texture(uMask, vST, 0).r) * texture2D(uIntensity, vST, 0));
}