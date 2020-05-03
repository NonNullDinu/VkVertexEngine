#pragma once

#include "GL/GraphicsContext.h"

namespace Vertex {
    class VulkanShaderPipeline;

    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        void Render() override;

        void SwapBuffers() override;

        void NotifyResize(int new_width, int new_height) override;

        void CleanUpContext() override;

        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool IsComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        inline VkInstance GetInstance() { return m_VkInstance; }
        inline VkDevice GetDevice() { return m_Device; }
        inline VkExtent2D GetSwapChainExtent() { return m_SwapChainExtent; }
        inline VkRenderPass GetRenderPass() { return m_RenderPass; }
        inline VkCommandBuffer GetCurrentCommandBuffer() { return m_CurrentCommandBuffer; }
        inline VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
        inline VkQueue GetQueue() { return m_GraphicsQueue; }
        inline VkDescriptorPool GetDescriptorPool() { return m_DescriptorPool; }
        inline std::vector<VkImage> GetSwapChainImages() { return m_SwapChainImages; }
        inline VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }
        inline uint32_t GetQueueFamily() { return FindQueueFamilies(m_PhysicalDevice).graphicsFamily.value(); }
        inline VkSurfaceKHR GetSurface() { return this->m_Surface; }

        static std::shared_ptr<VulkanContext> GetContext();
        
     private:
        GLFWwindow* m_WindowHandle;
        VkInstance m_VkInstance;

        VkDebugUtilsMessengerEXT debugMessenger;

        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_Device;
        VkSurfaceKHR m_Surface;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkSwapchainKHR m_SwapChain;
        std::vector<VkImage> m_SwapChainImages;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        std::vector<VkImageView> m_SwapChainImageViews;
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;

        VkPipelineLayout m_PipelineLayout;
        std::vector<std::reference_wrapper<VulkanShaderPipeline> > m_Pipelines;

        VkRenderPass m_RenderPass;
        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;
        size_t m_CurrentFrame = 0;

        VkDescriptorPool m_DescriptorPool;

        VkCommandBuffer m_CurrentCommandBuffer;


        void InitVulkan();
        void InitVulkanDebugger();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSurface();
        void CreateSwapChain();
        void CreateGraphicsPipelineLayout();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateFrameBuffers();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void CreateDescriptorPool();

        void CleanupSwapChain();
        void RecreateSwapChain();

        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        bool CheckValidationLayerSupport();

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        std::vector<const char*> GetRequiredExtensions();

        static std::shared_ptr<VulkanContext> s_Context;
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    };

}