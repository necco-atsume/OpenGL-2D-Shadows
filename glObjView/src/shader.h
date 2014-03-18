#pragma once

#include <GL/glew.h>
#include <string>

namespace sp
{
    GLchar *ReadSource(std::string path);
    GLuint CreateShader(std::string path, GLenum type);
    GLuint CreateProgram(std::string vertexPath, std::string fragmentPath, std::string geomPath);
}