#pragma once

#include <string>

#include <SDL.h>
#include <glm/glm.hpp>

#include "i_displayobject.h"
#include "rendercontext.h"

using namespace std;
namespace sp {
    ///A sprite object. Has a normal map and a ...
    class Sprite
        : public IDisplayObject {
    public:
        ///Creates a new Sprite object with default transforms and specified normal and color sprites.
        Sprite(const string& color, const string& normal)
            : colorPath(color), normalPath(normal) {
            _position = glm::vec2(0, 0);
        }

        //position x,y and <xy>

        virtual void Initialize();
        virtual void LoadResources();
        virtual void Update(float dt);
        virtual void Dispose();

        virtual void DrawColorLayer(const RenderContext &render);
        virtual void DrawNormalLayer(const RenderContext &render);

    private:
        //Just holds 00 01 10  01 10 11 vertices. They are 
        static const glm::vec3 vertices[6];

        glm::vec2 _position;

        string colorPath, normalPath;
        SDL_Surface 
        //TODO: If we do rotations and stuff, do a Transform object. But not for the demo stuff.
    };
}