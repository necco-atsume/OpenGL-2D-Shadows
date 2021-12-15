glShadows
=========

### Note: This was an old CS university assignment I did, and I don't have any confidence any of the code is useful here. I've posted a little writeup of how this works down at the end of the readme that might be useful though. 

OpenGL 2D normal map / shadow casting demo based loosely on https://github.com/mattdesl/lwjgl-basics/wiki/ShaderLesson6
This is my chosen CS 419 Shaders final project, and contains some really hacky code, but the shaders should be relatively sane.

Uses some art from OpenGameArt, and the cobblestone texture from http://kay-vriend.blogspot.com/2013/07/low-res-goodies-cobblestones.html. 

* L to add a light that's a clone of the one under the mouse.
* Up, Down to move the mouse light's z value
* Left, Right to change the light's radius multiplier (beware: strange effects can happen)
* Press R to randomize light color for the mouse light
* Q,W, A,S, Z,X to change constant linear quadratic falloff. for the mouse light

Screenshots
---

![Colored Lighting](http://i.imgur.com/4DdpJHH.jpg)
![Multiple Lights](http://i.imgur.com/WJtty89.jpg)
![Multiple Visible Shadows](http://i.imgur.com/mjWmXAZ.jpg)

### Note:
Since a few people have starred this repo in the last few weeks, I figured I should actually post some sort of explanation on how it works here (though I'm going from 8 year old memory here so gimme some slack though!)
I actually had a nice pdf of the writeup I turned in for this, but it's lost to time / bit rot so I gotta re-write this from memory. Oh well! :upside_down:

#### Building: 
This... probably doesn't build. It is "an exercise left up to the reader," but you _could maybe_ have some luck if you open the vcxproj in Visual Studio.

#### How (I think) it works:
```
You can refer to the code in src/renderer.cpp, and the various shaders that uses.

Draw the normal maps and diffuse color layers to a framebuffer containing both (in deferred rendering this is known as a "g-buffer").

Create an intermediate framebuffer for the lighting.

For each light in the scene
  For each object in the scene *
    Using the g-buffer created earlier, draw the normal map pass for this light (insert link to a far smarter person explaining normal mapping here.)
    Now draw the shadows for each light, first creating the shadow volume** with a geometry shader, then drawing it with a fragment shader.
      - Shadow volume Geometry Shader: (nb: this works on individual triangles)
          - For each point in the triangle, create a new point by pushing the original point outward along the vector that the point and the light make, past the edge of the screen.
          - Emit six triangles that cover the whole of the shadow volume with overlap (see: shaders/shadowgen.geom)
            (Now thinking about this eight years later, I think you only need to draw two triangles here and I'm almost sure I just brute forced this bit because due date?)
      - Geometry Shader, second pass: Remove the original triangle from the above shadow volume (so the actual object's not occluded by itself.)
      - Fragment (pixel) shader: Mark these pixels as occluded (r=1) ***
    Now add this to the intermediate framebuffer, treating any pixel that was marked as inside the shadow volume as black.
Finally, blend the diffuse color, light pass and ambient lighting together to make the final scene.

 
* This could be further optimized to only count objects that are able to hit by the light (maybe using a quadtree?)
** Well, it's 2d so it'd be a shadow area, right?
*** It's been quite some time since I actually did anything graphics related but I'm almost certain this is the exact reason that stencil buffers exist.
```
