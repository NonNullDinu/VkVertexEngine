#pragma once

#include <Renderer/GraphicsContext.h>
#include "Core.h"

#include "Event/Event.h"

namespace Vertex {

    class IWindow
    {
    public:
        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(std::function<void(Event&)> func) = 0;

        virtual void SetVSync(bool conf) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        virtual bool ShouldClose() const = 0;

        virtual GraphicsContext& GetGraphicsContext() const = 0;
    };

}
