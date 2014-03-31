#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "rendercontext.h"

namespace sp {
    class Emitter {
    public:
        Emitter(const glm::vec3& position, float radius, const glm::vec3& color) 
            : m_position(position), m_color(color), m_radius_px(radius) {
        }

        glm::vec3 position() const {
            return m_position;
        }
        void position(const glm::vec3& p) {
            m_position = p;
        }

        glm::vec3 color() const {
            return m_color;
        }
        void color(const glm::vec3& c) {
            m_color = c;
        }

        float radius() const {
            return m_radius_px;
        }
        void radius(const float r) {
            m_radius_px = r;
        }

        float constant() const {
            return m_falloff_constant;
        }
        void constant(const float r) {
            m_falloff_constant = r;
        }

        float linear() const {
            return m_falloff_linear;
        }
        void linear(const float r) {
            m_falloff_linear = r;
        }

        float quadratic() const {
            return m_falloff_quadratic;
        }
        void quadratic(const float r) {
            m_falloff_quadratic = r;
        }

        void Initialize();
        void Load();
        void Update();
        void Dispose();

        ///IntensityAt: Returns the distance between this light and a 2d point.
        ///Used to estimate light intensities to find the most influential lights
        ///hitting a point. 
        float IntensityAt(const glm::vec2& other) const;

        //Shadowing is done by drawing the light overlay to a (low resolution) render texture,
        //drawing all occluding geometry (using the geometry shader stuff)  to mask out shadows,
        //and drawing this back into a final light overlay framebuffer.
        //This is done in the Renderer class.

    private:
        glm::vec3 m_position;
        glm::vec3 m_color;
        float m_radius_px;

        //FIXME: Falloff values from https://github.com/mattdesl/lwjgl-basics/wiki/ShaderLesson6
        float m_falloff_constant = 0.4f;
        float m_falloff_linear = 3.0f;
        float m_falloff_quadratic = 20.0f;
    };
}