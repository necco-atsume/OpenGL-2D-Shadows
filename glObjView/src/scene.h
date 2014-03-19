#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "i_displayobject.h"
#include "emitter.h"
#include "rendercontext.h"

namespace sp {
    ///Scene: Collection of lights, DisplayObjects. (Abstract class.)
    class Scene {
    public:
        Scene() {
            //TODO: ctor stuff.
        }

        virtual void InitDisplayObjects() = 0;

        void Initialize();
        void Load();
        virtual void Update(float dt);
        void Dispose();

        //TODO: void GenerateOcclusionGeometry(); and 
        
        void BeginDraw();
        void DrawColorLayer(const RenderContext& c);
        void DrawShadowLayer(const RenderContext& shadow);

        vector<IDisplayObject*> objects;
        vector<Emitter*> lights;

    private:
        ///Sorts the emitters by brightness at a given point.
        void sortEmitters(const glm::vec2& point);
        void bindLights(const RenderContext& rc) const;
    };
}