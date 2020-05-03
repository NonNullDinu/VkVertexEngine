#include "Application.h"

namespace Vertex {

    Application* Application::s_AppInstance = nullptr;

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

#if defined(VX_RENDER_API_OPENGL)

        float vertices[21] = {
            -0.5f, -0.5f, 0.0f,   0.4f, 0.8f, 0.4f, 1.0f,
             0.5f, -0.5f, 0.0f,   0.4f, 0.8f, 0.4f, 1.0f,
            -0.5f,  0.5f, 0.0f,   0.4f, 0.8f, 0.4f, 1.0f
        };

        uint32_t indices[3] = { 0, 1, 2 };

        BufferLayout layout = { { ShaderDataType::Float3 }, { ShaderDataType::Float4 } };
          
        m_VertexArray.reset(VertexArray::Create());
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices), layout));
    
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        // --------------------------------------

        float vertices2[21] = {
             0.5f, -0.5f, 0.0f,   0.8f, 0.4f, 0.4f, 1.0f,
             0.5f,  0.5f, 0.0f,   0.8f, 0.4f, 0.4f, 1.0f,
            -0.5f,  0.5f, 0.0f,   0.8f, 0.4f, 0.4f, 1.0f
        };

        uint32_t indices2[3] = { 0, 1, 2 };

        BufferLayout layout2 = { { ShaderDataType::Float3 }, { ShaderDataType::Float4 } };

        m_VertexArray2.reset(VertexArray::Create());
        m_VertexBuffer2.reset(VertexBuffer::Create(vertices2, sizeof(vertices2), layout2));

        m_VertexArray2->AddVertexBuffer(m_VertexBuffer2);

        m_IndexBuffer2.reset(IndexBuffer::Create(indices2, sizeof(indices2)));

        m_VertexArray2->SetIndexBuffer(m_IndexBuffer2);

        const char* vertex_src = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec4 v_Color;

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
                v_Color = a_Color;
            }
        )";

        const char* fragment_src = R"(
            #version 330 core

            in vec4 v_Color;

            out vec4 o_Color;

            void main()
            {
                o_Color = v_Color;
            }
        )";

        m_Shader.reset(Shader::Create(vertex_src, fragment_src));

        // --------------------------------------
        // --------------------------------------
        // --------------------------------------

#endif // VX_RENDER_API_OPENGL

        s_AppInstance = this;

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Dispatch<EventTypes::WindowClose, WindowCloseEvent>(VX_BIND_FUNC_1(Application::OnWindowCloseEvent));

        m_Window->OnEvent(event);

        for (std::vector<Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
        }
    }

    void Application::Run()
    {
        m_Window->GetGraphicsContext().SetRenderCallback(VX_BIND_FUNC_0(Application::Render));

        while (m_Running)
        {
            // ------------- Temporary --------------
#ifdef VX_RENDER_API_OPENGL
            glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();

            m_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            m_VertexArray2->Bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer2->GetCount(), GL_UNSIGNED_INT, nullptr);
#endif
            // --------------------------------------

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            // --------------- ImGui ----------------

            m_ImGuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnImguiRender();
            
            m_ImGuiLayer->End();

            // --------------------------------------

            m_Window->OnUpdate();
        }

        return;
    }

    void Application::Render()
    {
        m_ImGuiLayer->Begin();

        for (Layer* layer : m_LayerStack)
            layer->OnImguiRender();

        m_ImGuiLayer->End();
    }

    // Application specific event callbacks
    bool Application::OnWindowCloseEvent(Event& event)
    {
        m_Running = false;
        return true;
    }

}
