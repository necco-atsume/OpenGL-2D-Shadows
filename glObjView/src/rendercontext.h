#pragma once

#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace std;
namespace sp {
    ///The Rendering context: Flyweight object used to interact with current opengl program. 
    ///DisplayObjects interact with the / through this.
    class RenderContext {
    public:
        RenderContext(GLuint program) : activeProgram(program) {
            glUseProgram(program);
        } 

        //PassThrough functions for gl equivalents: "glUniform()" calls.
        void BindTexture(const string& loc, GLenum active, const GLuint index) const;
        void BindVertices(GLuint buffer, size_t size) const;
        void BindValue(const string& loc, const float f) const;
        void BindValue(const string& loc, const bool b) const;
        void BindValue(const string& loc, const glm::vec2& v) const;
        void BindValue(const string& loc, const glm::vec3& v) const;
        void BindValue(const string& loc, const glm::mat3x3& m) const;

        void SetActive() {
            glUseProgram(activeProgram);
        }

        //TODO: We need to be able to swap framebuffer junk in here.
        //void PushFramebuffer(); ???

        void Draw(const size_t len) const;

    private:
        GLuint activeProgram;
    };
}