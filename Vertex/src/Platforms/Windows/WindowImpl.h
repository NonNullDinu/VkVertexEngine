#pragma once

#include "Core/Window.h"

namespace Vertex {

    struct WindowProperties
    {
        bool v_sync;
        const char* title;
        unsigned int width, height;

        void(*event_callback)(Event&);

        WindowProperties(const char* _title = "Vertex",
                         unsigned int _width = 1024,
                         unsigned int _height = 576,
                         bool _v_sync = true)
            : title(_title), width(_width), height(_height), v_sync(_v_sync)
        {
        }
    };

    class VX_PUBLIC_API Window : public ABCWindow
    {
    public:
        Window(const WindowProperties properties = WindowProperties());
        ~Window();

        void OnUpdate() override;

        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        inline void SetEventCallback(T&& func) override { m_Data.event_callback = &func; }

        inline void SetVSync(bool conf) override { glfwSwapInterval((conf) ? 1 : 0); m_Data.v_sync = conf; }
        inline bool IsVSync() const override { return m_Data.v_sync; }

        inline void* GetNativeWindow() const override { return m_Window; }

    private:
        GLFWwindow* m_Window;
        WindowProperties m_Data;
    };

}