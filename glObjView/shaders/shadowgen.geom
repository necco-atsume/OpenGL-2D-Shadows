#version 400 

uniform vec3 uLightPosition;
uniform mat3 uViewportTransform;
uniform bool uUnmask;

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

//For now we'll just assume framebuffer's 800x600...

//ShadowGen: Projects the fragments onto the light radius' circle, and emits
//triangles to cover the occluded area.
//This is drawn on top of a circle-gradient texture framebuffer.
//(alpha: lightStrength->0, rgb: lightColor)
//This gets blended with the /.
void main() {
    vec3 lp = vec3(uLightPosition.x, uLightPosition.y, 0);

    //FIXME: Something's going wrong with our clip coord transform.
    //This should just be uViewportTransform * lp
    vec2 l = (uViewportTransform * lp).xy + vec2(-1, 1);

    vec2 v0 = gl_in[0].gl_Position.xy;
    vec2 v1 = gl_in[1].gl_Position.xy;
    vec2 v2 = gl_in[2].gl_Position.xy;
    vec2 v3 = v0 + (normalize(v0 - l) * vec2(800, 800));
    vec2 v4 = v1 + (normalize(v1 - l) * vec2(800, 800));
    vec2 v5 = v2 + (normalize(v2 - l) * vec2(800, 800));

    gl_Position = vec4(0,0,0,1);

    gl_Position.xy = v0; EmitVertex();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v2; EmitVertex();
    EndPrimitive();
    if (uUnmask) return;

    gl_Position.xy = v0; EmitVertex();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v3; EmitVertex();
    EndPrimitive();

    gl_Position.xy = v3; EmitVertex();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v4; EmitVertex();
    EndPrimitive();
    gl_Position.xy = v1; EmitVertex();
    gl_Position.xy = v2; EmitVertex();
    gl_Position.xy = v4; EmitVertex();
    EndPrimitive();
    gl_Position.xy = v2; EmitVertex();
    gl_Position.xy = v4; EmitVertex();
    gl_Position.xy = v5; EmitVertex();
    EndPrimitive();
}