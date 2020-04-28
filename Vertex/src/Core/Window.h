#pragma once

#include "Core/Core.h"

#include "Core/Event/Event.h"

namespace Vertex {

    class Window
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
    };

    // utility class
    struct WindowProperties
    {
        bool v_sync;
        const char* title;
        unsigned int width, height;
        unsigned int gl_major_version, gl_minor_version;
        bool gl_use_compat;

        std::function<void(Event&)> event_callback;

        WindowProperties(const char* _title = "Vertex",
                         unsigned int _width = 1024,
                         unsigned int _height = 576,
                         bool _v_sync = true,
                         unsigned int _gl_major_version = 4,
                         unsigned int _gl_minor_version = 6,
                         bool _gl_use_compatibility_profile = false)
            : title(_title), width(_width), height(_height), v_sync(_v_sync), gl_major_version(_gl_major_version),
              gl_minor_version(_gl_minor_version), gl_use_compat(_gl_use_compatibility_profile)
        {
        }
    };

}
