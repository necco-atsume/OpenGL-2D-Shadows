glShadows
=========

OpenGL 2D normal map / shadow casting demo based loosely on https://github.com/mattdesl/lwjgl-basics/wiki/ShaderLesson6
This is my chosen CS 419 Shaders final project, and contains some really hacky code, but the shaders should be relatively sane.

(This probably won't build on other computers; I'll fix that in a bit.)

![Colored Lighting](http://i.imgur.com/4DdpJHH.jpg)
![Multiple Lights](http://i.imgur.com/WJtty89)
![Multiple Visible Shadows](http://i.imgur.com/mjWmXAZ)

Uses some art from OpenGameArt, and the cobblestone texture from http://kay-vriend.blogspot.com/2013/07/low-res-goodies-cobblestones.html. 
L to add a light that's a clone of the one under the mouse.
Up, Down to move the mouse light's z value
Left, Right to change the light's radius multiplier (beware: strange effects can happen)
Press R to randomize light color for the mouse light
Q,W, A,S, Z,X to change constant linear quadratic falloff. for the mouse light


