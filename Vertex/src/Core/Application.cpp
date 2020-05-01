#include "Application.h"

namespace Vertex {

    Application* Application::s_AppInstance = nullptr;

    static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
    {
        switch(type)
        {
            case ShaderDataType::Bool   : return GL_BOOL;
            case ShaderDataType::Mat3   : return GL_FLOAT;
            case ShaderDataType::Mat4   : return GL_FLOAT;
            case ShaderDataType::Int    : return GL_INT;
            case ShaderDataType::Int2   : return GL_INT;
            case ShaderDataType::Int3   : return GL_INT;
            case ShaderDataType::Int4   : return GL_INT;
            case ShaderDataType::Float  : return GL_FLOAT;
            case ShaderDataType::Float2 : return GL_FLOAT;
            case ShaderDataType::Float3 : return GL_FLOAT;
            case ShaderDataType::Float4 : return GL_FLOAT;
        }

        Logger::GetCoreLogger()->error("Unknown shader data type, cancelling...");
        return 0;
    }

    Application::Application()
        : m_Running(true)
    {
        VX_CORE_ASSERT((!s_AppInstance), "Application cannot be instantiated twice!");

        Input::Init();

        m_Window.reset(Window::Create());
        
        m_Window->SetEventCallback(VX_BIND_FUNC_1(Application::OnEvent));

        // --------------------------------------
        // ------------- Temporary --------------
        // --------- Will be abstracted ---------
        // --------------------------------------

        glGenVertexArrays(1, &m_VertexArr);
        glBindVertexArray(m_VertexArr);

        float vertices[9] = {
            -0.5f, -0.5f, 0.0f,  //  0.7f, 0.7f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.0f,  //  0.7f, 1.0f, 0.7f, 1.0f,
             0.0f,  0.5f, 0.0f,  //  1.0f, 0.7f, 0.7f, 1.0f
        };

        uint32_t indices[3] = { 0, 1, 2 };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = { { ShaderDataType::Float3 } };

        uint16_t index = 0;
        for (auto& elem : layout.GetElements())
        {
            glEnableVertexAttribArray(index);

            glVertexAttribPointer(index,
                elem.component_count,
                ShaderDataTypeToOpenGL(elem.type),
                elem.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)elem.offset
            );

            index++;
        }

        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

        const char* vertex_src = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        const char* fragment_src = R"(
            #version 330 core

            out vec4 o_Color;

            void main()
            {
                o_Color = vec4(0.7, 0.7, 1.0, 1.0);
            }
        )";

        m_Shader.reset(Shader::Create(vertex_src, fragment_src));

        m_Shader->Bind();
        dynamic_cast<OpenGLShader*>(m_Shader.get())->LoadUniform("color");
        m_Shader->Unbind();

        // --------------------------------------

        s_AppInstance = this;

        ImGuiLayer* m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
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

            glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();

            glBindVertexArray(m_VertexArr);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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