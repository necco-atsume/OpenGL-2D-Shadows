#version 150 compatibility

attribute vec3 aVertex;

varying vec2 vST;

void main() {
    vST = ((aVertex + 1) / 2).xy;
    gl_Position = vec4(aVertex.xy, 0, 1);
}