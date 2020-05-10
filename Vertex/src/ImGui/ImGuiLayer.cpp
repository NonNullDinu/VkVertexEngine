#include "ImGuiLayer.h"

#include "Core/Event/Event.h"

#if defined(VX_RENDER_API_VULKAN)
    #include "GL/Vulkan/VulkanContext.h"
#endif

namespace Vertex
{

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            style.WindowRounding = 0.1f;

        Application& app = Application::Get();

        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

#if defined(VX_RENDER_API_OPENGL)
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
#elif defined(VX_RENDER_API_VULKAN)
        std::shared_ptr<VulkanContext> context = VulkanContext::GetContext();
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = context->GetInstance();
        init_info.PhysicalDevice = context->GetPhysicalDevice();
        init_info.Device = context->GetDevice();
        init_info.QueueFamily = context->GetQueueFamily();
        init_info.Queue = context->GetQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = context->GetDescriptorPool();
        init_info.Allocator = nullptr;
        init_info.MinImageCount = context->GetSwapChainImages().size();
        init_info.ImageCount = context->GetSwapChainImages().size();
        init_info.CheckVkResultFn = [](VkResult result)->void
        {
            if (result != VK_SUCCESS)
            {
                CoreLogger::Get()->error("Something went wrong in dear imgui");
            }
        };
        ImGui_ImplVulkan_Init(&init_info, context->GetRenderPass());

        VkCommandBuffer command_buffer = VulkanContext::GetContext()->GetLoadCommandBuffer();

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(command_buffer, &begin_info);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        vkEndCommandBuffer(command_buffer);
        vkQueueSubmit(VulkanContext::GetContext()->GetQueue(), 1, &end_info, VK_NULL_HANDLE);

        vkDeviceWaitIdle(VulkanContext::GetContext()->GetDevice());
        ImGui_ImplVulkan_DestroyFontUploadObjects();
#endif
    }

    void ImGuiLayer::OnDetach()
    {
#if defined(VX_RENDER_API_OPENGL)
        ImGui_ImplOpenGL3_Shutdown();
#elif defined(VX_RENDER_API_VULKAN)
        ImGui_ImplVulkan_Shutdown();
#endif
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin()
    {
#if defined(VX_RENDER_API_OPENGL)
        ImGui_ImplOpenGL3_NewFrame();
#elif defined(VX_RENDER_API_VULKAN)
        ImGui_ImplVulkan_NewFrame();
#endif
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
        // Rendering
        ImGui::Render();
#if defined(VX_RENDER_API_OPENGL)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(VX_RENDER_API_VULKAN)
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), VulkanContext::GetContext()->GetCurrentCommandBuffer());
#endif
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
#ifdef VX_RENDER_API_OPENGL
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
#endif
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
#ifdef VX_RENDER_API_OPENGL
            glfwMakeContextCurrent(backup_current_context);
#endif
        }
    }

    void ImGuiLayer::OnImguiRender()
    {
        bool show = true;
        ShowDockSpace(&show);
        ImGui::Begin("Test win");
        ImGui::End();
    }

    void ImGuiLayer::ShowDockSpace(bool* p_open)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();
    }

}