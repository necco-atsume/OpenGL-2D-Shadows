#include "texture.h"

#include <unordered_map>

using namespace std;

std::unordered_map<string, GLuint> g_texture_cache;

//For now the display objects free their own textures.
//This means that as soon as one of them goes bye, the texture gets unloaded.
//This is a problem if we support removing display objects.
GLuint LoadTexture(const string& path) {
    //Cache stuff.
    auto tex = g_texture_cache.find(path);
    if (tex != g_texture_cache.end()) {
        return g_texture_cache[path];
    }

    SDL_Surface* texSurface = IMG_Load(path.c_str());
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SDL_LockSurface(texSurface);

    if (texSurface->format->BitsPerPixel != 32) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurface->w, texSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texSurface->pixels);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurface->w, texSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texSurface->pixels);
    }

    SDL_UnlockSurface(texSurface);

    //TODO: Set params.
    SDL_FreeSurface(texSurface);

    g_texture_cache[path] = texture;

    return texture;
}
