#include "renderer.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

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
        glDeleteBuffers(1, &m_quad_vbo);
        glDeleteProgram(pSprite);
        glDeleteProgram(m_blend_program);
        glDeleteProgram(m_light_pass_program);
        glDeleteProgram(m_light_accum_program);
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

        //Make fullscreen quad vbo.
        glm::vec3 verts[6] = {
            glm::vec3(-1, -1, 1),
            glm::vec3(1, -1, 1),
            glm::vec3(-1, 1, 1),
            glm::vec3(1, -1, 1),
            glm::vec3(-1, 1, 1),
            glm::vec3(1, 1, 1)
        };

        m_quad_vbo_size = sizeof(verts);

        glGenBuffers(1, &m_quad_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_quad_vbo_size, &verts[0], GL_STATIC_DRAW);

        initPrograms();
        initFrameBuffers();

        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }

    void Renderer::Update(float dt) {
        if (scene() != nullptr)
            scene()->Update(dt);
    }

    void Renderer::Draw() {
        glBindVertexArray(vertexArray);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (scene() != nullptr)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_light_accum_fbo);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindFramebuffer(GL_FRAMEBUFFER, m_sprite_gbuffer);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindFramebuffer(GL_FRAMEBUFFER, m_light_pass_fbo);
            glClear(GL_COLOR_BUFFER_BIT);

            scene()->BeginDraw();

            beginDrawSprite();

            auto spriteCtx = RenderContext(pSprite);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
            scene()->DrawColorLayer(spriteCtx);
            glDisable(GL_DEPTH_TEST);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            //This is kinda hacky because I decided to store the framebuffers in the render object.
            //Ideally I'd do more of this in the scene.
            for (auto light = scene()->lights.begin(); light != scene()->lights.end(); light++) {
                //Set the framebuffer.
                glBindFramebuffer(GL_FRAMEBUFFER, m_light_pass_fbo);
                glClear(GL_COLOR_BUFFER_BIT);
                auto lightCtx = RenderContext(m_light_pass_program);
                lightCtx.BindVertices(m_quad_vbo, m_quad_vbo_size);
                //Set the light uniforms
                lightCtx.BindValue("uLightColor", (*light)->color());
                lightCtx.BindValue("uLightPosition", (*light)->position());
                lightCtx.BindValue("uConstant", (*light)->constant());
                lightCtx.BindValue("uLinear", (*light)->linear());
                lightCtx.BindValue("uQuadratic", (*light)->quadratic());
                lightCtx.BindValue("uRadius", (*light)->radius());

                lightCtx.BindValue("uCameraPosition", glm::vec2(0, 0));

                lightCtx.BindTexture("uNormal", 0, m_sprite_gbuffer_normal);

                //Draw the textured quad pass.
                lightCtx.Draw(6);

                glBindFramebuffer(GL_FRAMEBUFFER, m_light_pass_mask_fbo);
                glClear(GL_COLOR_BUFFER_BIT);
                auto shadowCtx = RenderContext(m_shadow_program);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                //Draw the shadow layer over it.
                shadowCtx.BindValue("uLightPosition", (*light)->position());
                shadowCtx.BindValue("uUnmask", false);
                scene()->DrawShadowLayer(shadowCtx); 
                shadowCtx.BindValue("uUnmask", true);
                scene()->DrawShadowLayer(shadowCtx); 

                //This is where blurring the mask layer would go.

                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                //Draw to the accumulated light framebuffer.
                glBindFramebuffer(GL_FRAMEBUFFER, m_light_accum_fbo);
                auto lightCombine = RenderContext(m_light_accum_program);
                glUseProgram(m_light_accum_program);
                lightCombine.BindVertices(m_quad_vbo, m_quad_vbo_size);
                lightCombine.BindTexture("uIntensity", 0, m_light_pass_fbo_tex);
                lightCombine.BindTexture("uMask", 1, m_light_pass_mask_fbo_tex);
                lightCombine.Draw(6);
            }

            //Draw fullscreen quad.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(m_blend_program);
            auto blendCtx = RenderContext(m_blend_program);
            blendCtx.BindVertices(m_quad_vbo, m_quad_vbo_size);
            blendCtx.BindTexture("uColor", 0, m_sprite_gbuffer_color);
            blendCtx.BindTexture("uIntensity", 1, m_light_accum_fbo_tex);

            blendCtx.Draw(6);
        }
    } 
    
    void Renderer::initPrograms() {
        //Set the constant uniforms.
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        m_width = (float)viewport[2];
        m_height = (float)viewport[3];

        float vw = m_width;
        float vh = m_height;

        glm::mat3 displayTransform(2 / vw, 0, 0,
            0, 2 / -vh, 0,
            -1, 1, 1);

        //Init all shader programs.
        GLuint sprite_vsh = sp::CreateShader("shaders/sprite.vert", GL_VERTEX_SHADER);
        GLuint sprite_fsh = sp::CreateShader("shaders/sprite.frag", GL_FRAGMENT_SHADER);
        pSprite = glCreateProgram();
        glAttachShader(pSprite, sprite_vsh);
        glAttachShader(pSprite, sprite_fsh);
        glLinkProgram(pSprite);
        glUseProgram(pSprite);

        //And uniforms.
        GLint mp = glGetUniformLocation(pSprite, "uViewportTransform");
        glUniformMatrix3fv(mp, 1, GL_FALSE, glm::value_ptr(displayTransform));

        GLint cp = glGetUniformLocation(pSprite, "uCameraPosition");
        glUniform2f(cp, 0, 0);
        
        GLuint blend_vsh = sp::CreateShader("shaders/texturedquad.vert", GL_VERTEX_SHADER);
        GLuint blend_fsh = sp::CreateShader("shaders/finalblend.frag", GL_FRAGMENT_SHADER);
        m_blend_program = glCreateProgram();
        glAttachShader(m_blend_program, blend_vsh);
        glAttachShader(m_blend_program, blend_fsh);
        glLinkProgram(m_blend_program);
        glUseProgram(m_blend_program);
        //TODO:Bind blend stuff here.

        GLuint light_accum_vsh = sp::CreateShader("shaders/texturedquad.vert", GL_VERTEX_SHADER);
        GLuint light_accum_fsh = sp::CreateShader("shaders/shadowcombine.frag", GL_FRAGMENT_SHADER);
        m_light_accum_program = glCreateProgram();
        glAttachShader(m_light_accum_program, light_accum_vsh);
        glAttachShader(m_light_accum_program, light_accum_fsh);
        glLinkProgram(m_light_accum_program);
        glUseProgram(m_light_accum_program);
        //TODO: Bind lightaccum stuff here.


        GLuint light_pass_vsh = sp::CreateShader("shaders/lightgen.vert", GL_VERTEX_SHADER);
        GLuint light_pass_fsh = sp::CreateShader("shaders/lightgen.frag", GL_FRAGMENT_SHADER);
        m_light_pass_program = glCreateProgram();
        glAttachShader(m_light_pass_program, light_pass_vsh);
        glAttachShader(m_light_pass_program, light_pass_fsh);
        glLinkProgram(m_light_pass_program);
        glUseProgram(m_light_pass_program);

        GLuint shadow_vsh = sp::CreateShader("shaders/sprite.vert", GL_VERTEX_SHADER);
        GLuint shadow_fsh = sp::CreateShader("shaders/shadowgen.frag", GL_FRAGMENT_SHADER);
        GLuint shadow_gsh = sp::CreateShader("shaders/shadowgen.geom", GL_GEOMETRY_SHADER);
        m_shadow_program = glCreateProgram();
        glAttachShader(m_shadow_program, shadow_vsh);
        glAttachShader(m_shadow_program, shadow_fsh);
        glAttachShader(m_shadow_program, shadow_gsh);
        glLinkProgram(m_shadow_program);
        glUseProgram(m_shadow_program);

        mp = glGetUniformLocation(m_shadow_program, "uViewportTransform");
        glUniformMatrix3fv(mp, 1, GL_FALSE, glm::value_ptr(displayTransform));

        //TODO: Bind light pass constants here.
    }

    void Renderer::initFrameBuffers() {
        glGenFramebuffers(1, &m_sprite_gbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_sprite_gbuffer);

        //No need for precision.
        glGenTextures(1, &m_sprite_gbuffer_color);
        glBindTexture(GL_TEXTURE_2D, m_sprite_gbuffer_color);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_sprite_gbuffer_color, 0);

        glGenTextures(1, &m_sprite_gbuffer_normal);
        glBindTexture(GL_TEXTURE_2D, m_sprite_gbuffer_normal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glGenRenderbuffers(1, &m_sprite_gbuffer_render);
        glBindRenderbuffer(GL_RENDERBUFFER, m_sprite_gbuffer_render);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_sprite_gbuffer_render);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_sprite_gbuffer_normal, 0);

        GLenum spriteDrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, spriteDrawBuffers);

        //generate light occlusion pass framebuffer.
        //the one that we draw one light to
        glGenFramebuffers(1, &m_light_pass_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_light_pass_fbo);

        glGenTextures(1, &m_light_pass_fbo_tex);
        glBindTexture(GL_TEXTURE_2D, m_light_pass_fbo_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_light_pass_fbo_tex, 0);

        //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_light_pass_mask_fbo_tex, 0);
        GLenum lightDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, lightDrawBuffers);

        glGenFramebuffers(1, &m_light_pass_mask_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_light_pass_mask_fbo);

        glGenTextures(1, &m_light_pass_mask_fbo_tex); //This could probably be lower precision.
        glBindTexture(GL_TEXTURE_2D, m_light_pass_mask_fbo_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_light_pass_mask_fbo_tex, 0);
        GLenum lightMaskDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, lightMaskDrawBuffers);

        //generate lights framebuffer.
        //the one we recombine our stuff into.
        glGenFramebuffers(1, &m_light_accum_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_light_accum_fbo);

        glGenTextures(1, &m_light_accum_fbo_tex);
        glBindTexture(GL_TEXTURE_2D, m_light_accum_fbo_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_light_accum_fbo_tex, 0);
        GLenum lightAccumDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, lightAccumDrawBuffers);
    }

    void Renderer::beginDrawSprite() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_sprite_gbuffer);
        glUseProgram(pSprite);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}