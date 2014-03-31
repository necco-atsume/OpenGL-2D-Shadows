#pragma once

#include <glm/glm.hpp>

#include "rendercontext.h"

namespace sp {
    ///IDisplayObject: Interface for displayables.
    class IDisplayObject {
    public:
        virtual void Initialize() = 0;
        virtual void LoadResources() = 0;
        virtual void Update(float dt) = 0;
        virtual void Dispose() = 0;
 
        //position x,y and <xy>
        float x() {
            return m_position.x;
        }
        void x(float value) {
            m_position.x = value;
        }

        float y() {
            return m_position.y;
        }
        void y(float value) {
            m_position.y = value;
        }

        float z() {
            return m_position.z;
        }
        void z(float value) {
            m_position.z = value;
        }

        glm::vec2 position() const {
            return glm::vec2(m_position.x, m_position.y);
        }

        void position(const glm::vec2& p) {
            m_position = glm::vec3(p, m_position.z);
        }

        virtual void Draw(const RenderContext &render) = 0;
        virtual void DrawOcclusion(const RenderContext &render) = 0;

    protected: 
        glm::vec3 m_position;
    };
}