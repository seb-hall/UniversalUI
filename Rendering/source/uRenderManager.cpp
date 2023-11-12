//
// Created by sebam on 11/11/2023.
//

#include "../include/uRenderManager.h"
#include "../include/vulkan/vulkan.h"
#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstdio>

#if defined (_WIN32)
#include "../include/vulkan/vulkan_win32.h"
#elif defined(__linux__)
#include <vulkan/vulkan_xcb.h>
#elif defined(__ANDROID__)
    #include <vulkan/vulkan_android.h>
#endif

VkInstance uRenderManager::instance;
VkPhysicalDevice uRenderManager::physicalDevice;
VkDevice uRenderManager::device;
std::map<uWindowHandle, uRenderStuff> uRenderManager::renderStuffMap;

bool uRenderManager::Init() {

    printf("\nINFO: Vulkan initializing..");
    // Step 1: Initialize Vulkan
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Enable extensions required for Win32 integration
    std::vector<const char *> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();


    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
        return false;
    }

    printf("\nINFO: Vulkan instance created ok!");

    // Step 2: Enumerate and choose a physical device
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cerr << "No Vulkan-compatible devices found!" << std::endl;
        return false;
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    // Choose a physical device (you can implement your selection criteria here)
    VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;

    for (const auto &device : physicalDevices) {

        printf("\nDEVICE FOUND");

        // Check Vulkan compatibility
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        if (deviceProperties.apiVersion < VK_API_VERSION_1_0) {
            continue;  // Skip incompatible devices
        }
        //
        //  THIS IS BAD CODE
        //
        selectedDevice = device;

        // Check for required extensions support
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        bool extensionsSupported = true;
        for (const char *requiredExtension : extensions) {
            bool found = false;
            for (const auto &extension : availableExtensions) {
                if (strcmp(requiredExtension, extension.extensionName) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                extensionsSupported = false;
                break;
            }
        }

        if (!extensionsSupported) {
            continue;  // Skip devices that don't support required extensions
        }

        // Check for suitable queue families (graphics and presentation)
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        bool graphicsQueueFound = false;
        bool presentationQueueFound = false;

        for (uint32_t i = 0; i < queueFamilyCount; ++i) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueueFound = true;
                // Check if this queue family supports presentation to your Win32 surface
                VkBool32 presentationSupported = VK_FALSE;
                presentationSupported = vkGetPhysicalDeviceWin32PresentationSupportKHR(device, i);
                if (presentationSupported) {
                    presentationQueueFound = true;
                    break;
                }
            }
        }

        if (graphicsQueueFound && presentationQueueFound) {
            // Check for device features here if needed

            // The device meets the criteria; set selectedDevice and break
            selectedDevice = device;
            break;
        }
    }

    if (selectedDevice == VK_NULL_HANDLE) {
        std::cerr << "No suitable Vulkan-compatible device found!" << std::endl;
        return false;
    }

    physicalDevice = selectedDevice;

    // Specify the queue families you need (e.g., graphics queue, presentation queue)
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    //.queueFamilyIndex = graphicsQueueFamilyIndex; // Replace with your queue family index
    queueCreateInfo.queueCount = 1; // You may need multiple queues for different purposes
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // Specify the device features you need (optional)
    VkPhysicalDeviceFeatures deviceFeatures = {}; // Set the features you need

// Specify device extensions (e.g., for swap chain support)
    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


    // Create the logical device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1; // Set to the number of queue create infos
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 1; // Set to the number of extensions
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;


    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        // Handle device creation failure#
        std::cerr << "device creation failed" << std::endl;
        return false;
    }

    printf("\nINFO: Vulkan initialized ok!");


    return true;
}

void uRenderManager::SetupForWindow(uWindow* window) {
    // Step 2: Create a Vulkan surface
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hwnd = static_cast<HWND>(window->systemHandle);
    surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

    VkSurfaceKHR surface;
    if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        std::cerr << "Failed to create window surface!" << std::endl;
        return;
    }

    renderStuffMap[window->systemHandle].surface = surface;

    printf("\nINFO: Vulkan created surface ok!");

    // Query the capabilities of the surface
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    // Choose the surface format
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

    VkSurfaceFormatKHR chosenFormat = surfaceFormats[0]; // Choose a suitable format

    for (const auto& availableFormat : surfaceFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            chosenFormat = availableFormat;
            break;
        }
    }

    // Choose the presentation mode
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

    VkPresentModeKHR chosenPresentMode = VK_PRESENT_MODE_FIFO_KHR; // Choose a suitable mode

    for (const auto& availablePresentMode : presentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            chosenPresentMode = availablePresentMode;
            break;
        }
    }


    // Choose the swap extent (resolution)
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
        renderStuffMap[window->systemHandle].swapChainExtent = surfaceCapabilities.currentExtent;
    } else {
        // If the surface size is undefined, choose a suitable size
        int width, height;
        // Get the window size here (replace with actual window size retrieval)
        width = 1000;
        height = 750;
        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        // Clamp the extent to the available range
        actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width,
                                      std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height,
                                       std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));

        renderStuffMap[window->systemHandle].swapChainExtent = actualExtent;
    }

    // Determine the number of images in the swap chain
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // Create the swap chain
    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = chosenFormat.format;
    swapChainCreateInfo.imageColorSpace = chosenFormat.colorSpace;
    swapChainCreateInfo.imageExtent = renderStuffMap[window->systemHandle].swapChainExtent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = chosenPresentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE; // For swap chain recreation

    vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &renderStuffMap[window->systemHandle].swapChain);

    // Query the swap chain images
    vkGetSwapchainImagesKHR(device, renderStuffMap[window->systemHandle].swapChain, &imageCount, nullptr);
    renderStuffMap[window->systemHandle].swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, renderStuffMap[window->systemHandle].swapChain, &imageCount, renderStuffMap[window->systemHandle].swapChainImages.data());


    // Store the format and extent for later use
    renderStuffMap[window->systemHandle].swapChainImageFormat = chosenFormat.format;
    renderStuffMap[window->systemHandle].swapChainExtent = renderStuffMap[window->systemHandle].swapChainExtent;

    // setup swapchain imageviews
    renderStuffMap[window->systemHandle].swapChainImageViews.resize(imageCount);

    for (size_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = renderStuffMap[window->systemHandle].swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = chosenFormat.format;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &renderStuffMap[window->systemHandle].swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }



    // create renderpass
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = chosenFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderStuffMap[window->systemHandle].renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

    printf("\nGOT TO HERE OK 1");

    // setup framebuffers
    renderStuffMap[window->systemHandle].swapChainFramebuffers.resize(imageCount);

    for (size_t i = 0; i < imageCount; i++) {
        VkImageView attachments[] = {
                renderStuffMap[window->systemHandle].swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderStuffMap[window->systemHandle].renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = 1000;
        framebufferInfo.height = 750;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &renderStuffMap[window->systemHandle].swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }

}

void uRenderManager::RenderToWindow(uWindow *window) {

    printf("\nGOT TO HERE OK");

    // create renderpass

    // Example code for rendering a green color (replace with your actual rendering code):
    // Begin the command buffer recording

    //QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    //poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &renderStuffMap[window->systemHandle].commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = renderStuffMap[window->systemHandle].commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    printf("\nGOT TO HERE OK 0");

    if (vkAllocateCommandBuffers(device, &allocInfo, &renderStuffMap[window->systemHandle].commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    printf("\nGOT TO HERE OK 1");


    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    printf("\nGOT TO HERE OK 3");

    vkBeginCommandBuffer(renderStuffMap[window->systemHandle].commandBuffer, &beginInfo);
    printf("\nGOT TO HERE OK 2");
    // Set clear color to green
    VkClearValue clearColor = {0.0f, 1.0f, 0.0f, 1.0f};

    // Begin render pass
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderStuffMap[window->systemHandle].renderPass;
    renderPassInfo.framebuffer = renderStuffMap[window->systemHandle].framebuffer; // Your framebuffer
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = renderStuffMap[window->systemHandle].swapChainExtent; // Your swap chain extent
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;



    vkCmdBeginRenderPass(renderStuffMap[window->systemHandle].commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    printf("\nGOT TO HERE OK 2.5");

    // Bind pipeline layout, graphics pipeline, and draw
    vkCmdBindPipeline(renderStuffMap[window->systemHandle].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderStuffMap[window->systemHandle].graphicsPipeline);
    // Other binding and drawing commands as needed

    // End render pass
    vkCmdEndRenderPass(renderStuffMap[window->systemHandle].commandBuffer);

    // End command buffer recording
    vkEndCommandBuffer(renderStuffMap[window->systemHandle].commandBuffer);

    // Submit the command buffer for rendering
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderStuffMap[window->systemHandle].commandBuffer;

    vkQueueSubmit(renderStuffMap[window->systemHandle].queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderStuffMap[window->systemHandle].queue);

    // Clear command buffer for the next use
    vkResetCommandBuffer(renderStuffMap[window->systemHandle].commandBuffer, 0);
}