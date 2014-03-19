#pragma once
#include "sprite.h"

#include <string>

using namespace std;
namespace sp {
    //Background layer sprite.
    //Since we need a sprite to plop on the background layer.
    //This assumes that the camera never moves, the texture is 64x64 and that the screen size is 800x600 in order to cover it with 
    //a pattern. Basically, useless outside of the project.
    class Background 
        : public Sprite {
    public:
        Background(const string& color, const string& normal)
            : Sprite(color, normal) {}
        virtual void Draw(const RenderContext& render);
        virtual void DrawOcclusion(const RenderContext& render);
    };

}
