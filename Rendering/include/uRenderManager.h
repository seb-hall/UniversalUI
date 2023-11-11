//
// Created by sebam on 11/11/2023.
//

#ifndef URENDERMANAGER_H
#define URENDERMANAGER_H
#include "../include/vulkan/vulkan.h"
#include "uWindow.h"
class uRenderManager {
    static VkInstance instance;
public:

    static bool Init();

    static void CreateVulkanSurfaceForWindow(uWindow* window);

};


#endif //URENDERMANAGER_H
