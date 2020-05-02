#pragma once

#include "GL/GraphicsContext.h"

namespace Vertex {

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

        static std::shared_ptr<VulkanContext> GetContext();
        
     private:
        GLFWwindow* m_WindowHandle;
        VkInstance m_VkInstance;
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
        VkRenderPass m_RenderPass;
        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        VkSemaphore m_ImageAvailableSemaphore;
        VkSemaphore m_RenderFinishedSemaphore;
        VkDescriptorPool m_DescriptorPool;

        VkCommandBuffer m_CurrentCommandBuffer;


        void InitVulkan();
//        void InitVulkanDebugger();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSurface();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateFrameBuffers();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSemaphores();
        void CreateDescriptorPool();

        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        static std::shared_ptr<VulkanContext> s_Context;
    };

}