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
        RenderContext(GLuint program) : activeProgram(program) {} 

        //PassThrough functions for gl equivalents: "glUniform()" calls.
        void BindVertices(GLuint buffer, size_t size);
        void BindValue(const string& loc, const float f);
        void BindValue(const string& loc, const glm::vec2& v);
        void BindValue(const string& loc, const glm::vec3& v);
        void BindValue(const string& loc, const glm::mat3x3& m);

        //TODO: We need to be able to swap framebuffer junk in here.
        //void PushFramebuffer(); ???

        bool Draw(int len);

    private:
        GLuint activeProgram;
    };
}