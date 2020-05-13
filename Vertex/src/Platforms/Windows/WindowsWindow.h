#pragma once

#include "Core/Window.h"

#include "Core/Input.h"
#include "WindowsInput.h"

namespace Vertex
{

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProperties properties = WindowProperties());
        ~WindowsWindow();

        void OnUpdate() override;
        void OnEvent(Event& event) override;

        unsigned int GetWidth() const override { return m_Data.width; }
        unsigned int GetHeight() const override { return m_Data.height; }

        inline void SetEventCallback(std::function<void(Event&)> func) override { m_Data.event_callback = func; }

        inline void SetVSync(bool conf) override
        {
            glfwSwapInterval((conf) ? 1 : 0);
            m_Data.v_sync = conf;
        }

        inline bool IsVSync() const override { return m_Data.v_sync; }

        inline void* GetNativeWindow() const override { return m_Window; }

        inline bool ShouldClose() const override { return glfwWindowShouldClose(m_Window); }

        inline GraphicsContext& GetGraphicsContext() const override { return *m_Context; }

    private:
        GLFWwindow*                      m_Window;
        std::shared_ptr<GraphicsContext> m_Context;
        WindowProperties                 m_Data;

    private:
        void ShutDown();

        // event callbacks
        bool OnWindowResizeEvent(WindowResizeEvent& event);
    };

}