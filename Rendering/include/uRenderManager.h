//
// Created by sebam on 11/11/2023.
//

#ifndef URENDERMANAGER_H
#define URENDERMANAGER_H
#include "../include/vulkan/vulkan.h"
#include "uWindow.h"

#include <vector>
#include <map>

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
}

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct uRenderStuff {
    VkSurfaceKHR surface; // Your Vulkan surface
   // VkDevice device; // Your Vulkan device
    VkQueue queue; // Your Vulkan queue
    VkSwapchainKHR swapChain; // Your Vulkan swap chain
    std::vector<VkImage> swapChainImages; // Vector of swap chain images
    std::vector<VkImageView> swapChainImageViews; // Vector of swap chain images
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkFormat swapChainImageFormat; // Format of the swap chain images
    VkExtent2D swapChainExtent; // Extent (resolution) of the swap chain images
    VkRenderPass renderPass; // Your Vulkan render pass
    VkPipelineLayout pipelineLayout; // Your Vulkan pipeline layout
    VkPipeline graphicsPipeline; // Your Vulkan graphics pipeline
    VkCommandPool commandPool; // Your Vulkan command pool
    VkCommandBuffer commandBuffer; // Your Vulkan command buffer
    VkFramebuffer framebuffer;
};

class uRenderManager {
    static VkInstance instance;
    static VkPhysicalDevice physicalDevice;
    static VkDevice device;
    static std::map<uWindowHandle, uRenderStuff> renderStuffMap;
public:

    static bool Init();

    static void SetupForWindow(uWindow* window);

    static void RenderToWindow(uWindow* window);

    static void CreateVulkanInstance();

    static void PickPhysicalDevice();

    static bool IsDeviceSuitable(VkPhysicalDevice device);

    static void CreateLogicalDevice(uWindow* window);

    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, uWindow* window);

};


#endif //URENDERMANAGER_H
