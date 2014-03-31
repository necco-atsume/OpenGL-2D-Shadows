#include "scene.h"

#include <iostream>
#include <functional>

using namespace std;
namespace sp {
    void Scene::Initialize() {
        InitDisplayObjects();

        for (auto iter = objects.begin(); iter != objects.end(); iter++) {
            (*iter)->Initialize();
        }
    }

    void Scene::Load() {
        for (auto iter = objects.begin(); iter != objects.end(); iter++) {
            (*iter)->LoadResources();
        }
    }

    void Scene::Update(float dt) {
        for (auto iter = objects.begin(); iter != objects.end(); iter++) {
            (*iter)->Update(dt);
        }
    }

    void Scene::Dispose() {
        //Free objects.
        while (!objects.empty()) {
            IDisplayObject* obj = objects.back();
            obj->Dispose();
            delete obj;
            objects.pop_back();
        }
        while (!lights.empty()) {
            Emitter* e = lights.back();
            e->Dispose();
            delete e;
            lights.pop_back();
        }
    }

    void Scene::BeginDraw() {
        //TODO: Sort the display objects in here.
        sort(objects.begin(), objects.end(), [](IDisplayObject* a, IDisplayObject* b) -> bool { 
            return a->z() > b->z();
        });
    }

    void Scene::DrawColorLayer(const RenderContext &c) {
        for (auto iter = objects.begin(); iter != objects.end(); iter++) {
            auto obj = (*iter);
            obj->Draw(c);
        }
    }

    void Scene::DrawShadowLayer(const RenderContext& shadowpass) {

        for (auto disp = objects.begin(); disp != objects.end(); disp++) {
            auto obj = (*disp);
            obj->DrawOcclusion(shadowpass);
        }
        
    }

    //TODO: DrawLightLayers

    bool lightSortPred(const glm::vec2 dist, const Emitter* a, const Emitter* b) {
        auto ad = a->IntensityAt(dist);
        auto bd = b->IntensityAt(dist);
        return a->IntensityAt(dist) > b->IntensityAt(dist);
    }

    void Scene::sortEmitters(const glm::vec2& dist) {
        std::sort(lights.begin(), lights.end(), [dist](const Emitter* a, const Emitter* b) -> bool {
            return lightSortPred(dist, a, b);
        });
    }

    void Scene::bindLights(const RenderContext& c) const {
        //Get the most influential lights and set them in the shader.
        if (lights.size() > 0) {
            c.BindValue("uUseLight0", true);
            c.BindValue("uLightPosition0", lights[0]->position());
            c.BindValue("uLightColor0", lights[0]->color());

            if (lights.size() > 1) {
                c.BindValue("uUseLight1", true);
                c.BindValue("uLightPosition1", lights[1]->position());
                c.BindValue("uLightColor1", lights[1]->color());

                if (lights.size() > 2) {
                    c.BindValue("uUseLight2", true);
                    c.BindValue("uLightPosition2", lights[2]->position());
                    c.BindValue("uLightColor2", lights[2]->color());
                }
                else {
                    c.BindValue("uUseLight2", false);
                }
            }
            else {
                c.BindValue("uUseLight1", false);
                c.BindValue("uUseLight2", false);
            }
        }
        else {
            c.BindValue("uUseLight0", false);
            c.BindValue("uUseLight1", false);
            c.BindValue("uUseLight2", false);
        }
        auto x = glGetError();
    }
}