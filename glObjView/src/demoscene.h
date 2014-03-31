#pragma once
#include "emitter.h"
#include "scene.h"

namespace sp {
    class DemoScene
        : public Scene {
    public:
        DemoScene() { }
        virtual void InitDisplayObjects();
        virtual void Update(float dt);
    private:
        Emitter* m_mouse_emitter;
    };
}
