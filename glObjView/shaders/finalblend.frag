#version 150 compatibility

uniform sampler2D uColor, uIntensity;

varying vec2 vST;

out vec4 FragColor;

void main() {
    FragColor.rgb = texture(uColor, vST, 0).rgb * (vec3(.01,.02,.05) + texture2D(uIntensity, vST, 0).rgb);
    FragColor.a = 1;
}
