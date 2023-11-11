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

    printf("\nINFO: Vulkan initialized ok!");

    return true;
}

void uRenderManager::CreateVulkanSurfaceForWindow(uWindow* window) {
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

    printf("\nINFO: Vulkan created surface ok!");

    return;
}