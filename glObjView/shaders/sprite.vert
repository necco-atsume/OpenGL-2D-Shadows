#version 150 compatibility 

attribute vec3 aVertex;

uniform mat3 uViewportTransform;
uniform vec2 uCameraPosition;
uniform vec3 uPosition;
uniform sampler2D tSprite;

varying vec2 vST;
varying vec2 vPositionUnits;

void main() {
    vec2 size = textureSize(tSprite, 0);
    vec3 pxPosition = vec3(size.x * aVertex.x + uPosition.x,
                           size.y * aVertex.y + uPosition.y, 1);
    pxPosition.xy -= uCameraPosition;

    gl_Position = vec4((uViewportTransform * pxPosition).xy, uPosition.z, 1);

    //FIXME: This doesn't account for bits  outside of the camera view.
    vPositionUnits = pxPosition.xy;

    vST = vec2(aVertex.x, aVertex.y);
}