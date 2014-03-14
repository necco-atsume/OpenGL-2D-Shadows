#include "scene.h"

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
        for (auto iter = objects.begin(); iter != objects.end(); iter++) {
            (*iter)->Dispose();
        }
    }

    void Scene::BeginDraw() {
        //TODO: Sort the display objects in here.
    }

    void Scene::DrawColorLayer(const RenderContext &c) {
        for (auto iter = objects.begin(); iter != objects.end(); iter++) {
            (*iter)->DrawColorLayer();
        }
    }

    void Scene::DrawNormalLayer(const RenderContext &c) {
    }

    //TODO: DrawLightLayers
}