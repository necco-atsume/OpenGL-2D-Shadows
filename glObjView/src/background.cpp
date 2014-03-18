#include "background.h"

#include <glm/glm.hpp>

namespace sp {
    void Background::Draw(const RenderContext& rc) {
        auto vp = glm::vec3(0, 0, 1);
        rc.BindTexture("tSprite", 0, this->tSprite);
        rc.BindTexture("tNormal", 1, this->tNormal);
        rc.BindVertices(vbo, sizeof(vertices));
        for (int x = 0; x < 800; x += 128) {
            for (int y = 0; y < 600; y += 128) {
                vp.x = x;
                vp.y = y;
                rc.BindValue("uPosition", vp);
                rc.Draw(6);
            }
        }
    }
}