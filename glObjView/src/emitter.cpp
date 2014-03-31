#include "emitter.h"

#include <iostream>

using namespace std;
namespace sp {
    void Emitter::Initialize() {}
    void Emitter::Load() { }
    void Emitter::Update() { }
    void Emitter::Dispose() {} 

    float Emitter::IntensityAt(const glm::vec2& other) const {
        glm::vec2 mp(m_position.x, m_position.y);
        float distance = glm::distance(mp, other);
        float nd = glm::min(distance / m_radius_px, 1.0f);

        return 1.0f / (m_falloff_constant + (nd * m_falloff_linear) + (nd * nd * m_falloff_quadratic));
    }

}