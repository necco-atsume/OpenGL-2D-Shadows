#version 150 compatibility

uniform float uConstant, uLinear, uQuadratic;
uniform float uRadius;

uniform vec3 uLightPosition, uLightColor;

uniform sampler2D uNormal;

varying vec2 vST;
varying vec2 vPositionUnits;

layout(location=0) out vec4 Color;

void main() {
    vec3 vp = vec3(gl_FragCoord.x, 600 - gl_FragCoord.y , 0);

    //Find the point we're at.
    float distance = length(vec3(vp) - uLightPosition);
    float dn = (distance / uRadius );

    float intensity = 1 / (uConstant + (dn * uLinear) + (dn * dn * uQuadratic));

    vec3 nm = texture2D(uNormal, vST, 0).rgb;

    vec3 n = normalize((2 * nm) - 1);
    vec3 l = normalize(uLightPosition - vp);
    
    float diffuseStrength = max(0, dot(l, n));

    Color.rgb = uLightColor * diffuseStrength;
    Color.a = clamp(intensity, 0, 1);
}