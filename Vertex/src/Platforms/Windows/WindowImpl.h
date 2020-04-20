#pragma once

#include <GLFW/glfw3.h>
#include "Core/Window.h"

namespace Vertex {

    struct WindowProperties
    {
        bool v_sync;
        const char* title;
        unsigned int width, height;

        std::function<void(Event&)> event_callback;

        WindowProperties(const char* _title = "Vertex",
                         unsigned int _width = 1024,
                         unsigned int _height = 576,
                         bool _v_sync = true)
            : title(_title), width(_width), height(_height), v_sync(_v_sync)
        {
        }
    };

    class Window : public IWindow
    {
    public:
        Window(const WindowProperties properties = WindowProperties());
        ~Window();

        void OnUpdate() override;

        unsigned int GetWidth() const override { return m_Data.width; }
        unsigned int GetHeight() const override { return m_Data.height; }

        inline void SetEventCallback(std::function<void(Event&)> func) override { m_Data.event_callback = func; }

        inline void SetVSync(bool conf) override { glfwSwapInterval((conf) ? 1 : 0); m_Data.v_sync = conf; }
        inline bool IsVSync() const override { return m_Data.v_sync; }

        inline void* GetNativeWindow() const override { return m_Window; }

    private:
        GLFWwindow* m_Window;
        WindowProperties m_Data;

    private:
        void ShutDown();
    };

}