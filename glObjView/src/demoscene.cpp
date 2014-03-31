#include "demoscene.h"

#include <glm/glm.hpp>
#include <SDL.h>

#include "background.h"
#include "emitter.h"
#include "sprite.h"



namespace sp {
    bool g_add_light_hacky_debounce = true;
    bool g_add_crate_hacky_debounce = true;
    void DemoScene::InitDisplayObjects() {
        auto sprite = new Sprite("resources/mtl_o_c.png", "resources/mtl_o_n_e.png");
        objects.push_back(sprite);
        sprite->x(100);
        sprite->y(400);
        sprite->z(0.2);

        auto sprite2 = new Sprite("resources/mtl_o_c.png", "resources/mtl_o_n_e.png");
        objects.push_back(sprite2);
        sprite2->x(100);
        sprite2->y(100);
        sprite2->z(0.1);

        auto sprite3 = new Sprite("resources/mtl_o_c.png", "resources/mtl_o_n_e.png");
        objects.push_back(sprite3);
        sprite3->x(500);
        sprite3->y(100);
        sprite3->z(0.1);

        auto sprite4 = new Sprite("resources/mtl_o_c.png", "resources/mtl_o_n_e.png");
        objects.push_back(sprite4);
        sprite4->x(500);
        sprite4->y(400);
        sprite4->z(0.1);

        for (float xp = 0; xp < 800; xp += 256) {
            for (float yp = 0; yp < 600; yp += 256) {
                auto bg = new Background("resources/stone_color.png", "resources/stone_normal.png");
                bg->x(xp);
                bg->y(yp);
                objects.push_back(bg);
            }
        }

        m_mouse_emitter = new Emitter(glm::vec3(200, 200, 10), 800, glm::vec3(1, 0.9, 0.9));

        lights.push_back(m_mouse_emitter);
    }

    void DemoScene::Update(float dt) {
        glm::vec3 mp(m_mouse_emitter->position());

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        mp.x = mx;
        mp.y = my;

        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_UP]) {
            mp.z += 0.5;
        }

        if (state[SDL_SCANCODE_DOWN]) {
            mp.z -= 0.5;
        }

        if (state[SDL_SCANCODE_LEFT]) {
            m_mouse_emitter->radius((m_mouse_emitter->radius() - 100.));
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            m_mouse_emitter->radius((m_mouse_emitter->radius() + 100.));
        }
        if (state[SDL_SCANCODE_Q]) {
            m_mouse_emitter->quadratic((m_mouse_emitter->quadratic() - 1.));
        }
        if (state[SDL_SCANCODE_W]) {
            m_mouse_emitter->quadratic((m_mouse_emitter->quadratic() + 1.));
        }
        if (state[SDL_SCANCODE_A]) {
            m_mouse_emitter->linear((m_mouse_emitter->linear() - 1.));
        }
        if (state[SDL_SCANCODE_S]) {
            m_mouse_emitter->linear((m_mouse_emitter->linear() + 1.));
        }
        if (state[SDL_SCANCODE_Z]) {
            m_mouse_emitter->constant((m_mouse_emitter->constant() - 0.10));
        }
        if (state[SDL_SCANCODE_X]) {
            m_mouse_emitter->constant((m_mouse_emitter->constant() + 0.10));
        }

        if (state[SDL_SCANCODE_R]) {
            float r, g, b;
            r = static_cast<float> (rand() / (static_cast<float>(RAND_MAX)));
            g = static_cast<float> (rand() / (static_cast<float>(RAND_MAX)));
            b = static_cast<float> (rand() / (static_cast<float>(RAND_MAX)));
            m_mouse_emitter->color(glm::vec3(r, g, b));
        }

        if (state[SDL_SCANCODE_L]) {
            if (g_add_light_hacky_debounce) {
                auto light = new Emitter(*m_mouse_emitter);
                lights.push_back(light);
                g_add_light_hacky_debounce = false;
            }
        }
        else {
            g_add_light_hacky_debounce = true;
        }

        /*
        if (state[SDL_SCANCODE_O]) {
            if (g_add_crate_hacky_debounce) {
                g_add_crate_hacky_debounce = false;
                auto crate = new Sprite("resources/mtl_o_c.png", "resources/mtl_o_n_e.png");
                objects.push_back(crate);
                crate->x(mx);
                crate->y(my);
                crate->z(0.1);
            }
        }
        else {
            g_add_crate_hacky_debounce = true;
        }
        */

        m_mouse_emitter->position(mp);
    }
}