#include "VulkanContext.h"
#include "VulkanShaderPipeline.h"
#include "VulkanExtensions.h"

namespace Vertex
{

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        void* p_user_data)
    {

        CoreLogger::Get()->debug("validation layer: {}", p_callback_data->pMessage);

        return VK_FALSE;
    }

    constexpr int MAX_FRAMES_IN_FLIGHT = 4;
    static const std::vector<const char*> VulkanDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const std::vector<const char*> ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
#ifdef VX_CONFIGURATION_DEBUG
    constexpr bool EnableValidationLayers = true;
#else
    constexpr bool EnableValidationLayers = false;
#endif
    ;

    VulkanContext* VulkanContext::s_Context = nullptr;
    VulkanContext::VulkanContext(GLFWwindow* window)
        : m_WindowHandle(window)
    {
        VX_CORE_ASSERT(s_Context == nullptr, "Cannot have multiple vulkan contexts at once");
        InitVulkan();
        LoadVkExtensions(m_VkInstance);
        if constexpr (EnableValidationLayers)
        {
            InitVulkanDebugger();
        }
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipelineLayout();
        CreateFrameBuffers();
        CreateCommandPool();
        CreateCommandBuffers();
        CreateSyncObjects();
        CreateDescriptorPool();

        s_Context = this;
        CoreLogger::Get()->info("Created the Vulkan Context");
    }

    VulkanContext::~VulkanContext()
    {
        CoreLogger::Get()->info("Cleaning up the Vulkan Context");
        CleanUpContext();
        s_Context = nullptr;
    }

    void VulkanContext::Render()
    {
        vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        uint32_t image_index;

        VkResult result = vkAcquireNextImageKHR(m_Device,
                                                m_SwapChain,
                                                UINT64_MAX,
                                                m_ImageAvailableSemaphores[m_CurrentFrame],
                                                VK_NULL_HANDLE,
                                                &image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            VX_CORE_ASSERT(false, "cannot acquire next image");
        }

        m_PrepareRenderCallback(this);

        if (m_ImagesInFlight[image_index] != VK_NULL_HANDLE)
        {
            vkWaitForFences(m_Device, 1, &m_ImagesInFlight[image_index], VK_TRUE, UINT64_MAX);
        }
        m_ImagesInFlight[image_index] = m_InFlightFences[m_CurrentFrame];

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore wait_semaphores[] = {m_ImageAvailableSemaphores[m_CurrentFrame]};
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_CommandBuffers[image_index];

        VkSemaphore signal_semaphores[] = {m_RenderFinishedSemaphores[m_CurrentFrame]};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optional
        begin_info.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(m_CommandBuffers[image_index], &begin_info) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkBeginCommandBuffer failed");
        }

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_RenderPass;
        render_pass_info.framebuffer = m_SwapChainFramebuffers[image_index];

        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_SwapChainExtent;

        VkClearValue clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(m_CommandBuffers[image_index], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        m_CurrentCommandBuffer = m_CommandBuffers[image_index];

        m_RenderCallback(this);

        vkCmdEndRenderPass(m_CommandBuffers[image_index]);

        if (vkEndCommandBuffer(m_CommandBuffers[image_index]) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkEndCommandBuffer failed");
        }

        vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]);

        if (vkQueueSubmit(m_GraphicsQueue, 1, &submit_info, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkQueueSubmit failed");
        }

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        VkSwapchainKHR swap_chains[] = {m_SwapChain};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swap_chains;
        present_info.pImageIndices = &image_index;

        present_info.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(m_PresentQueue, &present_info);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkQueuePresentKHR failed");
        }
        vkQueueWaitIdle(m_PresentQueue);

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanContext::SwapBuffers()
    {
        if (m_NeedSwapChainRecreate)
        {
            RecreateSwapChain();
            m_NeedSwapChainRecreate = false;
        }
    }
    void VulkanContext::NotifyResize(int new_width, int new_height)
    {
        m_NeedSwapChainRecreate = true;
    }

    void VulkanContext::CleanupSwapChain()
    {
        vkDeviceWaitIdle(m_Device);

        for (size_t i = 0; i < m_SwapChainFramebuffers.size(); i++)
        {
            vkDestroyFramebuffer(m_Device, m_SwapChainFramebuffers[i], nullptr);
        }

        vkFreeCommandBuffers(m_Device, m_CommandPool, static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers
            .data());

        for (VulkanShaderPipeline& pipelinew : m_Pipelines)
        {
            pipelinew.CleanUp();
        }

        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
        {
            vkDestroyImageView(m_Device, m_SwapChainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
    }

    void VulkanContext::RecreateSwapChain()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(m_WindowHandle, &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(m_WindowHandle, &width, &height);
            glfwWaitEvents();
        }

        CoreLogger::Get()->debug("Resizing swap chain for {}x{}", width, height);

        vkDeviceWaitIdle(m_Device);

        CleanupSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipelineLayout();
        CreateFrameBuffers();
        CreateCommandBuffers();
    }

    void VulkanContext::CleanUpContext()
    {
        vkDeviceWaitIdle(m_Device);

        vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
        }

        CleanupSwapChain();

        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

        vkDestroyDevice(m_Device, nullptr);
        if constexpr (EnableValidationLayers)
        {
            vkDestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
        UnloadVkExtensions(m_VkInstance);
        vkDestroyInstance(m_VkInstance, nullptr);
    }

    void VulkanContext::InitVulkan()
    {
        if constexpr (EnableValidationLayers)
        {
            if (!CheckValidationLayerSupport())
            {
                VX_CORE_ASSERT(false, "validation layers requested, but not available");
            }
        }

        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "Vertex Engine";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Vertex Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;

        auto req_extensions = GetRequiredExtensions();
        create_info.enabledExtensionCount = static_cast<uint32_t>(req_extensions.size());
        create_info.ppEnabledExtensionNames = req_extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
        if constexpr (EnableValidationLayers)
        {
            create_info.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
            create_info.ppEnabledLayerNames = ValidationLayers.data();

            PopulateDebugMessengerCreateInfo(debug_create_info);
            create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
        }
        else
        {
            create_info.enabledLayerCount = 0;

            create_info.pNext = nullptr;
        }

        if (vkCreateInstance(&create_info, nullptr, &m_VkInstance) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateInstance failed");
        }

        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> extensions(extension_count);

        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    }

    void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info)
    {
        create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = VulkanDebugCallback;
    }

    void VulkanContext::InitVulkanDebugger()
    {
        VkDebugUtilsMessengerCreateInfoEXT create_info;

        PopulateDebugMessengerCreateInfo(create_info);

        if (vkCreateDebugUtilsMessengerEXT(m_VkInstance, &create_info, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateDebugUtilsMessengerEXT failed");
        }
    }

    void VulkanContext::PickPhysicalDevice()
    {
        m_PhysicalDevice = VK_NULL_HANDLE;
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_VkInstance, &device_count, nullptr);
        VX_CORE_ASSERT(device_count != 0, "no GPUs with Vulkan support");
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_VkInstance, &device_count, devices.data());

        for (const auto& device : devices)
        {
            if (IsDeviceSuitable(device))
            {
                m_PhysicalDevice = device;
                break;
            }
        }

        VX_CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "failed to find a suitable GPU!");
    }

    void VulkanContext::CreateLogicalDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> unique_queue_families = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queue_priority = 1.0f;
        for (uint32_t queue_family : unique_queue_families)
        {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkPhysicalDeviceFeatures device_features{};
        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        create_info.pQueueCreateInfos = queue_create_infos.data();

        create_info.pEnabledFeatures = &device_features;

        create_info.enabledExtensionCount = static_cast<uint32_t>(VulkanDeviceExtensions.size());
        create_info.ppEnabledExtensionNames = VulkanDeviceExtensions.data();

        create_info.enabledLayerCount = 0;

        if (vkCreateDevice(m_PhysicalDevice, &create_info, nullptr, &m_Device) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateDevice failed");
        }

        vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
    }

    VulkanContext::QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

        int i = 0;
        for (const auto& queue_family : queue_families)
        {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &present_support);
            if (present_support)
            {
                indices.presentFamily = i;
            }

            if (indices.IsComplete())
            {
                break;
            }

            i++;
        }
        return indices;
    }
    VulkanContext::SwapChainSupportDetails VulkanContext::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &format_count, nullptr);

        if (format_count != 0)
        {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &format_count, details.formats.data());
        }
        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &present_mode_count, nullptr);

        if (present_mode_count != 0)
        {
            details.presentModes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &present_mode_count, details.presentModes
                .data());
        }

        return details;
    }

    void VulkanContext::CreateSurface()
    {
        if (glfwCreateWindowSurface(m_VkInstance, m_WindowHandle, nullptr, &m_Surface) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "glfwCreateWindowSurface failed");
        }
    }

    void VulkanContext::CreateSwapChain()
    {
        SwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(m_PhysicalDevice);

        VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat(swap_chain_support.formats);
        VkPresentModeKHR present_mode = ChooseSwapPresentMode(swap_chain_support.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swap_chain_support.capabilities);

        uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

        if (swap_chain_support.capabilities.maxImageCount > 0
            && image_count > swap_chain_support.capabilities.maxImageCount)
        {
            image_count = swap_chain_support.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_Surface;

        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
        uint32_t queue_family_indices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        }
        else
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0; // Optional
            create_info.pQueueFamilyIndices = nullptr; // Optional
        }
        create_info.preTransform = swap_chain_support.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_Device, &create_info, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateSwapchainKHR failed");
        }

        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &image_count, nullptr);
        m_SwapChainImages.resize(image_count);
        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &image_count, m_SwapChainImages.data());

        m_SwapChainImageFormat = surface_format.format;
        m_SwapChainExtent = extent;
    }

    void VulkanContext::CreateGraphicsPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = 0; // Optional
        pipeline_layout_info.pSetLayouts = nullptr; // Optional
        pipeline_layout_info.pushConstantRangeCount = 0; // Optional
        pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(m_Device, &pipeline_layout_info, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreatePipelineLayout failed");
        }
    }

    void VulkanContext::CreateImageViews()
    {
        m_SwapChainImageViews.resize(m_SwapChainImages.size());

        for (size_t i = 0; i < m_SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_SwapChainImages[i];

            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_SwapChainImageFormat;

            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device, &create_info, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
            {
                VX_CORE_ASSERT(false, "vkCreateImageView failed");
            }
        }
    }

    void VulkanContext::CreateRenderPass()
    {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = m_SwapChainImageFormat;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;

        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;

        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;

        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        if (vkCreateRenderPass(m_Device, &render_pass_info, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateRenderPass failed");
        }
    }

    void VulkanContext::CreateFrameBuffers()
    {
        m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());
        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] = {
                m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebuffer_info{};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = m_RenderPass;
            framebuffer_info.attachmentCount = 1;
            framebuffer_info.pAttachments = attachments;
            framebuffer_info.width = m_SwapChainExtent.width;
            framebuffer_info.height = m_SwapChainExtent.height;
            framebuffer_info.layers = 1;

            if (vkCreateFramebuffer(m_Device, &framebuffer_info, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS)
            {
                VX_CORE_ASSERT(false, "vkCreateFramebuffer failed");
            }
        }
    }

    void VulkanContext::CreateCommandPool()
    {
        QueueFamilyIndices queue_family_indices = FindQueueFamilies(m_PhysicalDevice);

        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.queueFamilyIndex = queue_family_indices.graphicsFamily.value();
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

        if (vkCreateCommandPool(m_Device, &pool_info, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateCommandPool failed");
        }
    }

    void VulkanContext::CreateCommandBuffers()
    {
        {
            m_CommandBuffers.resize(m_SwapChainFramebuffers.size());

            VkCommandBufferAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.commandPool = m_CommandPool;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandBufferCount = (uint32_t)m_CommandBuffers.size();

            if (vkAllocateCommandBuffers(m_Device, &alloc_info, m_CommandBuffers.data()) != VK_SUCCESS)
            {
                VX_CORE_ASSERT(false, "vkAllocateCommandBuffers failed");
            }
        }
        {
            // allocate the load command buffer
            VkCommandBufferAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.commandPool = m_CommandPool;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandBufferCount = (uint32_t)1;

            if (vkAllocateCommandBuffers(m_Device, &alloc_info, &m_LoadCommandBuffer) != VK_SUCCESS)
            {
                VX_CORE_ASSERT(false, "vkAllocateCommandBuffers failed");
            }
        }
    }

    void VulkanContext::CreateSyncObjects()
    {
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(m_SwapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(m_Device, &semaphore_info, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_Device, &semaphore_info, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_Device, &fence_info, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                VX_CORE_ASSERT(false, "failed to create synchronization objects for a frame");
            }
        }
    }

    void VulkanContext::CreateDescriptorPool()
    {
        std::vector<VkDescriptorPoolSize> pool_sizes =
            {
                {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
            };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * pool_sizes.size();
        pool_info.poolSizeCount = (uint32_t)pool_sizes.size();
        pool_info.pPoolSizes = pool_sizes.data();

        if (vkCreateDescriptorPool(m_Device, &pool_info, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            VX_CORE_ASSERT(false, "vkCreateDescriptorPool failed");
        }
    }

    bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensions_supported = CheckDeviceExtensionSupport(device);

        bool swap_chain_adequate = false;
        if (extensions_supported)
        {
            SwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(device);
            swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.presentModes.empty();
        }

        return indices.IsComplete() && extensions_supported && swap_chain_adequate;
    }

    bool VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(VulkanDeviceExtensions.begin(), VulkanDeviceExtensions.end());

        for (const auto& extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    bool VulkanContext::CheckValidationLayerSupport()
    {
        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        for (const char* layer_name : ValidationLayers)
        {
            bool layer_found = false;

            for (const auto& layer_properties : available_layers)
            {
                if (strcmp(layer_name, layer_properties.layerName) == 0)
                {
                    layer_found = true;
                    break;
                }
            }

            if (!layer_found)
            {
                return false;
            }
        }

        return true;
    }

    VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
    {
        for (const auto& available_format : available_formats)
        {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB
                && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return available_format;
            }
        }

        return available_formats[0];
    }

    VkPresentModeKHR VulkanContext::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes)
    {
        for (const auto& available_present_mode : available_present_modes)
        {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return available_present_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width = 0, height = 0;
            glfwGetFramebufferSize(m_WindowHandle, &width, &height);
            VkExtent2D actual_extent = {(uint32_t)width, (uint32_t)height};

            actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent
                                                                                           .width,
                                                                                       actual_extent.width));
            actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent
                                                                                             .height,
                                                                                         actual_extent.height));

            return actual_extent;
        }
    }
    VulkanContext* VulkanContext::GetContext()
    {
        return s_Context;
    }

    std::vector<const char*> VulkanContext::GetRequiredExtensions()
    {
        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

        std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

        if constexpr (EnableValidationLayers)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        return extensions;
    }

}
