#version 150

uniform sampler2D uColor, uDiffuse, uAttenuation;

varying vec2 vST;

out vec4 FragColor;

void main() {
    vec3 color =   texture2D(uColor, vST).rgb;
    vec3 diffuse = texture2D(uDiffuse, vST).rgb;

    vec3 ambient = vec3(0.3, 0.3, 0.3);
    
    FragColor.rgb = color * (diffuse + ambient);
    FragColor.a = 1;
}
