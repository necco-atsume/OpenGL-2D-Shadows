#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "i_displayobject.h"
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
        void Update(float dt);
        void Dispose();

        //TODO: void GenerateOcclusionGeometry(); and 
        
        void BeginDraw();
        void DrawColorLayer(const RenderContext &c);
        void DrawNormalLayer(const RenderContext &c);

    private:
        vector<IDisplayObject*> objects;
    };
}