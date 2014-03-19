#include "shader.h"

#include <cstdio>
#include <string>

#include <GL/glew.h>


namespace sp
{

    //Allocates a buffer, using new, then reads the shader source into it.
    GLchar *ReadSource(std::string path)
    {
        FILE *file;
#ifdef _WIN32
        fopen_s(&file, path.c_str(), "rb");
#else
        file = fopen(path.c_str(), "rb");
#endif
        fseek(file, 0L, SEEK_END);
        size_t fileSize = ftell(file);

        fseek(file, 0L, SEEK_SET);

        GLchar *srcBuffer = new char[fileSize + 1];

        fread(srcBuffer, sizeof(GLchar), fileSize, file);

        srcBuffer[fileSize] = 0;
        fclose(file);

        return srcBuffer;
    }

    ///Creates a shader object, and returns it.
    GLuint CreateShader(std::string path, GLenum shaderType)
    {
        GLuint shader;
        const char* source = ReadSource(path);

        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        char compileResult[256];
        GLsizei len;
        glGetShaderInfoLog(shader, 255, &len, compileResult);
        compileResult[255] = '\0';

        printf("%s:\n", path.c_str());
        if (strcmp(compileResult, "") == 0) {
#ifdef _WIN32
            strcpy_s(compileResult, 255, "sp::CreateShader: success! :)");
#else
            strcpy(compileResult, "success! :)");
#endif
        }

        printf("---\n%s\n---\n\n", compileResult);

        delete[] source;
        return shader;
    }

    GLuint CreateProgram(std::string vertexPath, std::string fragmentPath, std::string geomPath)
    {
        GLuint vsh, fsh;

        const char *vsh_src = ReadSource(vertexPath);
        const char *fsh_src = ReadSource(fragmentPath);

        char buf[256];
        GLsizei len;

        vsh = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsh, 1, &vsh_src, NULL);
        glCompileShader(vsh);
        glGetShaderInfoLog(vsh, 256, &len, buf);
        printf("\n---\n\n%s\n", buf);

        //glGetShaderInfo doesn't return anything on success on 
#ifndef _WIN32
        printf("glGetShaderInfo() doesn't return anything on success. Not sure about on failure. \nIf you don't see anything else compilation probably worked :)");
#endif

        fsh = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsh, 1, &fsh_src, NULL);
        glCompileShader(fsh);
        glGetShaderInfoLog(fsh, 256, &len, buf);
        printf("%s\n---\n", buf);

        GLuint program = glCreateProgram();
        glAttachShader(program, vsh);
        glAttachShader(program, fsh);
        glLinkProgram(program);

        delete[] vsh_src;
        delete[] fsh_src;

        return program;
    }
}
