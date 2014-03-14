#pragma once 

#include <GL/glew.h>

#include "scene.h"

namespace sp {

    ///Renderer: Handles all the weird opengl stuff.
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void scene(Scene* s);
        Scene* scene();

        bool ready() {
            return _ready;
        }
        void ready(bool r) {
            _ready = r;
        }

        void Initialize();
        void Load();
        void Update(float dt);
        void Draw();

    private:
        Scene* _scene;
        bool _ready;

        //FIXME: Getter/Setters?
        GLuint pSprite, pSpriteNormal;
        GLuint vertexArray;
    };
}