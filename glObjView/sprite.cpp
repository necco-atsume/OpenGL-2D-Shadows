#include "sprite.h"

#include <SDL.h>
#include <SDL_image.h>

using namespace std;
namespace sp {

    void Sprite::Initialize() {
        //TODO:
    }

    void Sprite::LoadResources() {

    }

    void Sprite::Update(float dt) {
    }

    void Sprite::DrawColorLayer(const RenderContext& rc) {
    }

    void Sprite::DrawNormalLayer(const RenderContext& rc) {
    }

    //The vertex array we're gonna draw.
    //We're just drawing two tris so we can take care of other stuff in the vertex shader.
    const glm::vec3 Sprite::vertices[6] = {
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 1, 1)
    };

}