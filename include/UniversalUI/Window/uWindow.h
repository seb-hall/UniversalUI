//  uWindow.h - UniversalUI/Windowing
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef UWINDOW_H
#define UWINDOW_H

#include "UniversalUI/Misc/Types.h"
#include "UniversalUI/Misc/Geometry.h"
#include "UniversalUI/Window/uWindowHandle.h"

#include "wgpu/wgpu.h"


class uWindow {
public:

    uWindowHandle handle;
    uSize size;

    bool forceSmoothResize;

    int displayWidth;
    int displayHeight;


    uWindow(string title, uSize size);

    void PollEvents();

    virtual void Resized(uSize newSize);
    virtual void Render();

private:

    //  0 - untested, -1 error, 1 success
    static int POSTResult;


    WGPUInstance instance;
    WGPUAdapter adapter;
    WGPUDevice device;

    WGPUSurface surface;
    WGPUSwapChain swapchain;

    static void RequestAdapterCallback(WGPURequestAdapterStatus status,
        WGPUAdapter adapter, char const* message,
        void* userdata);
    static void RequestDeviceCallback(WGPURequestDeviceStatus status,
        WGPUDevice device, char const* message,
        void* userdata);

};




#endif // UWINDOW_H