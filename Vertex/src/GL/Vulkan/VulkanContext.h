#pragma once
#include <Renderer/GraphicsContext.h>

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

        static VkInstance GetInstance();
        static VulkanContext GetContext();

        inline VkDevice GetDevice() { return m_Device; }
        inline VkExtent2D GetSwapChainExtent() { return m_SwapChainExtent; }
        inline VkRenderPass GetRenderPass() { return m_RenderPass; }
     private:
        GLFWwindow* m_WindowHandle;
        VkInstance m_VkInstance;
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_Device;
        VkSurfaceKHR m_Surface;
        VkQueue m_PresentationQueue;
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

        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        static VkInstance* s_VkInstance;
        static VulkanContext *s_Context;
    };
}