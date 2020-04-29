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

//        glGenVertexArrays(1, &m_VertexArr);
//        glBindVertexArray(m_VertexArr);

        float vertices[9] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
//
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        uint32_t indices[3] = { 0, 1, 2 };

        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

//        const char* vertex_src = R"(
//            #version 330 core
//
//            layout(location = 0) in vec4 a_Position;
//
//            void main()
//            {
//                gl_Position = a_Position;
//            }
//        )";
//
//        const char* fragment_src = R"(
//            #version 330 core
//
//            layout(location = 0) out vec4 o_Color;
//
//            uniform vec3 color;
//
//            void main()
//            {
//                o_Color = vec4(color, 1.0);
//            }
//        )";
//
//        m_Shader.reset(Shader::Create(vertex_src, fragment_src));
//
//        m_Shader->Bind();
//        dynamic_cast<OpenGLShader*>(m_Shader.get())->LoadUniform("color");
//        m_Shader->Unbind();

        // --------------------------------------
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Dispatch<EventTypes::WindowClose, WindowCloseEvent>(VX_BIND_FUNC_1(Application::OnWindowCloseEvent));
        handler.Dispatch<EventTypes::WindowResize, WindowResizeEvent>(VX_BIND_FUNC_1(Application::OnWindowResizeEvent));

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

//            glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT);

//            m_Shader->Bind();
//            (*dynamic_cast<OpenGLShader*>(m_Shader.get()))["color"] = glm::vec3(0.2f, 0.8f, 0.9f);

//            glBindVertexArray(m_VertexArr);
//            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            // --------------------------------------

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

    bool Application::OnWindowResizeEvent(WindowResizeEvent& event)
    {
        m_Window->GetGraphicsContext().NotifyResize(event.GetWidth(), event.GetHeight());
        return true;
    }

}