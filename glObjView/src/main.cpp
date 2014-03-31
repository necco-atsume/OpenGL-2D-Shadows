#include "main.h"

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <SDL.h>
#include <SDL_image.h>

#include "demoscene.h"
#include "renderer.h"
#include "scene.h"

using namespace std;
using namespace sp;
int main(int argc, char** argv)
{
    Scene* scene = new DemoScene();
    Renderer r;
    
    r.scene(scene);

    r.Initialize();

    SDL_Window* window;
    bool running = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

    SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    r.Load();

    //Explicit setup stuff here.

    SDL_GL_SetSwapInterval(1);
    while (running) {
        SDL_Event polledEvent;
        while (SDL_PollEvent(&polledEvent)) {
            //Event handling here.
            if (polledEvent.type == SDL_QUIT) {
                running = false;
                break;
            }

        } 
        r.Update(0.16f); //FIXME: A constant 60fps is assumed.
        r.Draw();
        //And wait for vblank??
        SDL_GL_SwapWindow(window);
    }

    delete scene;
    SDL_GL_DeleteContext(window);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
