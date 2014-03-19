#pragma once

#include <string>

#include <SDL.h>
#include <GL/glew.h>
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
            m_position = glm::vec3(0, 0, 0.4);
        }

        virtual void Initialize();
        virtual void LoadResources();
        virtual void Update(float dt);
        virtual void Dispose();

        virtual void Draw(const RenderContext &render);
        virtual void DrawOcclusion(const RenderContext &render);

    protected:
        //Just holds 00 01 10  01 10 11 vertices.  //Transformed by the vs.
        static const glm::vec3 vertices[6];

        //This gets set / unset for each sprite instance.
        //This isn't optimal but it's not worth it to reference-count a 
        //shared vbo for each. 
        GLuint vbo;

        string colorPath, normalPath;

        GLuint tSprite, tNormal;
        //TODO: If we do rotations and stuff, do a Transform object. But not for the demo stuff.
    };
}