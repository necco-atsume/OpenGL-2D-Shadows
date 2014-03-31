#include "rendercontext.h"

#include <string>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
namespace sp {


    void RenderContext::BindVertices(GLuint vbo, size_t size) const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLuint location = glGetAttribLocation(activeProgram, "aVertex");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    void RenderContext::BindTexture(const string& loc, GLuint active, const GLuint tex) const {
        GLuint location = glGetUniformLocation(activeProgram, loc.c_str());
        glActiveTexture(GL_TEXTURE0 + active);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(location, active);
    }

    void RenderContext::BindValue(const string& loc, const float f) const {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform1f(ul, f);
    }

    void RenderContext::BindValue(const string& loc, const bool b) const {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform1i(ul, b);
    }

    void RenderContext::BindValue(const string& loc, const glm::vec2& v) const {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform2f(ul, v.x, v.y);
    }

    void RenderContext::BindValue(const string& loc, const glm::vec3& v) const {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniform3f(ul, v.x, v.y, v.z);
    }

    void RenderContext::BindValue(const string& loc, const glm::mat3x3& m) const {
        GLint ul = glGetUniformLocation(activeProgram, loc.c_str());
        glUniformMatrix3fv(ul, 1, GL_FALSE, glm::value_ptr(m));
    }

    void RenderContext::Draw(const size_t count) const { //Note that this is the sizeof size. might have to change
        glDrawArrays(GL_TRIANGLES, 0, count);
    }
}