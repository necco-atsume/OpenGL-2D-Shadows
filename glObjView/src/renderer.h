#pragma once 

#include <GL/glew.h>
#include <glm/glm.hpp>

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
        void initFrameBuffers();
        void initPrograms();

        void beginDrawSprite();
        void beginDrawLight();

        //TODO:
        //void genFramebuffer(const GLuint* fbo) const;
        //void genTexture(const GLuint* tex) const;

        Scene* _scene;
        bool _ready;
        
        //FIXME: Getter/Setters?
        GLuint pSprite, m_blend_program, m_light_pass_program, m_light_accum_program;
        GLuint m_shadow_program;
        GLuint vertexArray;

        int m_width, m_height;

        GLuint m_quad_vbo;
        size_t m_quad_vbo_size;

        GLuint m_sprite_gbuffer;
        GLuint m_sprite_gbuffer_render;
        GLuint m_sprite_gbuffer_normal;
        GLuint m_sprite_gbuffer_color;

        GLuint m_light_accum_fbo;
        GLuint m_light_accum_fbo_tex;

        GLuint m_light_pass_fbo;
        GLuint m_light_pass_fbo_tex;

        GLuint m_light_pass_mask_fbo;
        GLuint m_light_pass_mask_fbo_tex;
    };
}