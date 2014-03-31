#include "sprite.h"

#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"

using namespace std;
namespace sp {

    void Sprite::Initialize() {
    }

    void Sprite::LoadResources() {
        //LoadTexture helper function.
        tSprite = LoadTexture(colorPath);
        tNormal = LoadTexture(normalPath);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    void Sprite::Update(float dt) {
    }

    void Sprite::Dispose() {
        glDeleteTextures(1, &tSprite);
        glDeleteTextures(1, &tNormal);
    }

    void Sprite::Draw(const RenderContext& rc) {
        rc.BindVertices(vbo, sizeof(vertices));
        rc.BindValue("uPosition", this->m_position);

        rc.BindTexture("tSprite", 0, this->tSprite);
        rc.BindTexture("tNormal", 1, this->tNormal);

        rc.Draw(6); //2 tris
    }

    void Sprite::DrawOcclusion(const RenderContext &rc) {
        rc.BindVertices(vbo, sizeof(vertices));
        rc.BindValue("uPosition", this->m_position);

        rc.BindTexture("tSprite", 0, this->tSprite);
        rc.BindTexture("tNormal", 1, this->tNormal);

        rc.Draw(6); //2 tris
    }

    //void Sprite::DrawOverlayMask(const RenderContext& rc) {
    //}

    //The vertex array we're gonna draw.
    //We're just drawing two tris so we can take care of other stuff in the vertex shader.
    const glm::vec3 Sprite::vertices[6] = {
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 1, 1)
    };

}