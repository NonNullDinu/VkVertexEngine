#pragma once
#include <Renderer/GraphicsContext.h>

namespace Vertex {
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);

        void SwapBuffers() override;

        void NotifyResize(int new_width, int new_height) override;

        void CleanUpContext() override;
    private:
        GLFWwindow* m_WindowHandle;
        VkInstance m_VkInstance;
    };
}