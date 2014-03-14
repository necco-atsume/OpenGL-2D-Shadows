#include "main.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <SDL.h>

#include <GL/glew.h>

#include "shader.h"

using namespace std;
int main(int argc, char** argv)
{
    SDL_Window* window;
    bool running = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    window = SDL_CreateWindow("demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

    SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    //Explicit setup stuff here.
    
    glClearColor(1, 1, 1, 1);

    glm::vec4 vtxbuf[] = {
        glm::vec4(0, 0, 0, 1),
        glm::vec4(1, 0, 0, 1),
        glm::vec4(0, 1, 0, 1)
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint tribuf;
    glGenBuffers(1, &tribuf);
    glBindBuffer(GL_ARRAY_BUFFER, tribuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 3, &vtxbuf, GL_STATIC_DRAW);

    GLuint vsh = sp::CreateShader("shaders/sprite.vert", GL_VERTEX_SHADER);
    GLuint fsh = sp::CreateShader("shaders/sprite.frag", GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    glLinkProgram(program);
    glUseProgram(program);


    while (running) {
        SDL_Event polledEvent;
        while (SDL_PollEvent(&polledEvent)) {
            //Event handling here.
            if (polledEvent.type == SDL_QUIT) {
                running = false;
                break;
            }

        } 
    
        Draw(window, program, tribuf);
    }

    SDL_GL_DeleteContext(window);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

//temp: draw stuff.
void Draw(SDL_Window *window, GLuint program, GLuint buffer) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    auto loc = glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    SDL_GL_SwapWindow(window);
}




