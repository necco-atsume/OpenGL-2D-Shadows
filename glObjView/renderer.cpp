#include "renderer.h"

#include <GL/glew.h>

#include "rendercontext.h"
#include "scene.h"
#include "shader.h"

namespace sp {

    Renderer::Renderer() {
        _scene = nullptr;
    }

    Renderer::~Renderer() {
        if (scene() != nullptr) {
            scene(nullptr);
        }

        glDeleteProgram(pSprite);
        glDeleteProgram(pSpriteNormal);
    }

    Scene* Renderer::scene() {
        return _scene;
    }

    void Renderer::scene(Scene* s) {
        if (s == nullptr && scene() != nullptr) {
            _scene->Dispose();

            if (ready()) { s->Initialize(); }
            if (ready()) { s->Load(); }
        }
        _scene = s;
    }

    //Stuff that may be called before the gl context is ready.
    void Renderer::Initialize() {
        if (scene() != nullptr) 
            scene()->Initialize();
    }

    //Precond the SDL window / gl context are already made.
    void Renderer::Load() {
        if (scene() != nullptr) 
            scene()->Load();
 
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glClearColor(1, 1, 1, 1);

        //Init all shader programs.
        GLuint sprite_vsh = sp::CreateShader("shaders/sprite.vert", GL_VERTEX_SHADER);
        GLuint sprite_fsh = sp::CreateShader("shaders/sprite.vert", GL_FRAGMENT_SHADER);
        pSprite = glCreateProgram();
        glAttachShader(pSprite, sprite_vsh);
        glAttachShader(pSprite, sprite_fsh);
        glLinkProgram(pSprite);

        //And the normalmap one.
    }

    void Renderer::Update(float dt) {
        if (scene() != nullptr)
            scene()->Update(dt);
    }

    void Renderer::Draw() {
        glBindVertexArray(vertexArray);

        if (scene() != nullptr)
        {
            //TODO: Draw normalmap layer to rendertexture and bind for ...
            auto spriteCtx = RenderContext(pSprite);
            glUseProgram(pSprite);
            scene()->DrawColorLayer(spriteCtx);

            //Then light+occlusion layers pass.
            //To acc buffer
            //and blur pass blend it to the color layer (multiply).
        }
    } 
    

}