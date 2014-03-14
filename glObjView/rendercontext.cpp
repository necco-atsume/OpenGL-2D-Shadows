#include "rendercontext.h"

#include <string>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
namespace sp {

    void RenderContext::BindVertices(GLuint vbo, size_t size) {
        GLuint location = glGetAttribLocation(activeProgram, "aPosition");
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    void RenderContext::BindValue(const string& loc, const float f) {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform1f(ul, f);
    }

    void RenderContext::BindValue(const string& loc, const glm::vec2& v) {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform2f(ul, v.x, v.y);
    }

    void RenderContext::BindValue(const string& loc, const glm::vec3& v) {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform3f(ul, v.x, v.y, v.z);
    }

    void RenderContext::BindValue(const string& loc, const glm::mat3x3& m) {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniformMatrix3fv(ul, 1, GL_FALSE, glm::value_ptr(m));
    }

    void RenderContext::Draw() {
    }
}