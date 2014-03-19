#include "background.h"

#include <glm/glm.hpp>

namespace sp {

    void Background::Draw(const RenderContext& rc) {
        auto vp = glm::vec3(0, 0, 1);
        rc.BindTexture("tSprite", 0, this->tSprite);
        rc.BindTexture("tNormal", 1, this->tNormal);
        rc.BindVertices(vbo, sizeof(vertices));
        rc.BindValue("uPosition", m_position);
        rc.Draw(6);
    }

    //Background doesn't cast shadows.
    void Background::DrawOcclusion(const RenderContext& rc) { }
}