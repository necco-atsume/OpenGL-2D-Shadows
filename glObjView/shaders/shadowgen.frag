#version 150 compatibility

uniform bool uUnmask;

layout(location=0)out vec4 Mask;

//Mask out the triangles.
void main() {
    if (uUnmask) {
        Mask.rgb = vec3(0,1,1);
        Mask.a = 1;
    }
    else {
        Mask.rgb = vec3(1,0,0);
        Mask.a = 1;
    }
}