#include "Application.h"

namespace Vertex {

    Application* Application::s_AppInstance = nullptr;

    Application::Application()
        : m_Running(true)
    {
        VX_CORE_ASSERT((!s_AppInstance), "Application cannot be instantiated twice!");

#if defined(_WIN32)
        Input::Create(new WindowsInput());

        m_Window.reset(new WindowsWindow());

#elif defined(__linux__)
        Input::Create(new LinuxInput());

        m_Window.reset(new LinuxWindow());
        
#endif // _WIN32
        
        m_Window->SetEventCallback(VX_BIND_FUNC_1(Application::OnEvent));

        s_AppInstance = this;

        ImGuiLayer* m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        // --------------------------------------
        // ------------- Temporary --------------
        // --------- Will be abstracted ---------
        // --------------------------------------

        glGenVertexArrays(1, &m_VertexArr);
        glBindVertexArray(m_VertexArr);

        glGenBuffers(1, &m_VertexBuf);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuf);

        float vertices[9] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glGenBuffers(1, &m_IndexBuf);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuf);

        unsigned int indices[3] = { 0, 1, 2 };

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

        const char* vertex_src = R"(
            #version 330 core

            layout(location = 0) in vec4 a_Position;

            void main()
            {
                gl_Position = a_Position;
            }
        )";

        const char* fragment_src = R"(
            #version 330 core

            layout(location = 0) out vec4 o_Color;

            void main()
            {
                o_Color = vec4(0.0, 1.0, 0.7, 1.0);
            }
        )";

        m_Shader.reset(new Shader(vertex_src, fragment_src));

        // --------------------------------------
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Dispatch<EventTypes::WindowClose, WindowCloseEvent>(VX_BIND_FUNC_1(Application::OnWindowCloseEvent));

        for (std::vector<Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // ------------- Temporary --------------

            glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArr);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            // --------------------------------------

            if (Input::IsKeyPressed(VX_KEY_SPACE))
                Logger::GetCoreLogger()->debug("Space bar pressed");

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnImguiRender();
            
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }

        return;
    }

    // Application specific event callbacks
    bool Application::OnWindowCloseEvent(Event& event)
    {
        m_Running = false;
        return true;
    }

}