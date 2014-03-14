#pragma once

#include <glm/glm.hpp>

#include "rendercontext.h"

namespace sp {
    ///IDisplayObject: Interface for displayables.
    class IDisplayObject {
    public:
        virtual void Initialize() = 0;
        virtual void LoadResources() = 0;
        virtual void Update(float dt) = 0;
        virtual void Dispose() = 0;
 
        virtual void DrawColorLayer(RenderContext &render) = 0;
        virtual void DrawNormalLayer(RenderContext &render) = 0; //TODO: closest 3 non-occluded lights
    };
}