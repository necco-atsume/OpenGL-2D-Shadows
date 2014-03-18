#version 150

uniform sampler2D tSprite, tNormal;
uniform vec2 uCameraPosition;

uniform bool uUseLight0, uUseLight1, uUseLight2;
uniform vec3 uLightPosition0, uLightPosition1, uLightPosition2;
uniform vec3 uLightColor0, uLightColor1, uLightColor2;

varying vec2 vST;
varying vec2 vPositionUnits;

layout(location=0) out vec4 Color;
layout(location=1) out vec4 Diffuse;

void main() {
    vec2 ts = textureSize(tSprite, 0);
    vec3 color = texture2D(tSprite, vST).rgb;
    vec3 pos = vec3(vPositionUnits, 0);
    //vec3 pos = vec3(gl_FragCoord.x, 600-gl_FragCoord.y, 0);

    //Get the normal map.
    vec3 nm = texture2D(tNormal, vST).rgb;
    //Since we're working in camera pixel coordinate space, the normal map's y values
    //don't actually need to be flipped, even though they're wrong for OpenGL.
    nm = (2 * nm) - 1;
    vec3 n = normalize(nm);

    vec3 diffuse0 = vec3(0,0,0);
    vec3 diffuse1 = vec3(0,0,0);
    vec3 diffuse2 = vec3(0,0,0);

    if (uUseLight0) {
        vec3 lp0 = uLightPosition0 - vec3(uCameraPosition, 0);
        vec3 l0 = normalize(lp0 - pos);
        float ds0 = max(0, dot(l0, n));
        diffuse0 = uLightColor0 * ds0;
    }

    if (uUseLight1) {
        vec3 lp1 = uLightPosition1 - vec3(uCameraPosition, 0);
        vec3 l1 = normalize(lp1 - pos);
        float ds1 = max(0, dot(l1, n));
        diffuse1 = uLightColor1 * ds1;  
    }

    if (uUseLight2) {
        vec3 lp2 = uLightPosition2 - vec3(uCameraPosition, 0);
        vec3 l2 = normalize(lp2 - pos);
        float ds2 = max(0, dot(l2, n));
        diffuse2 = uLightColor2 * ds2;       
    }

    Diffuse.rgb = (diffuse0 + diffuse1 + diffuse2);
    Diffuse.a = 1;

    Color = texture2D(tSprite, vST);
    Color.a = 1;
}