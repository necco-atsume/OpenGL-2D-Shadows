#version 150 compatibility

uniform sampler2D tSprite, tNormal;
uniform vec2 uCameraPosition;

varying vec2 vST;
varying vec2 vPositionUnits;

layout(location=0) out vec4 Color;
layout(location=1) out vec4 Normal;

//Passthru
void main() {
    Normal.rgb = texture2D(tNormal, vST).rgb;
    Normal.a = 1;

    Color.rgb = texture2D(tSprite, vST).rgb;
    Color.a = 1;
}