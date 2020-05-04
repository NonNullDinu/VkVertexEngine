#include "ImGuiLayer.h"

#include "Core/Event/Event.h"

static void ShowDockSpace(bool* p_open);

namespace Vertex {

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
        init_info.CheckVkResultFn = [](VkResult result) -> void
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
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImguiRender()
    {
        bool show = true;
        ShowDockSpace(&show);
        ImGui::Begin("Test win");
        ImGui::End();
    }
}

// from imgui_demo.h
void ShowDockSpace(bool* p_open)
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                        | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}