//  uWindow.h - UniversalUI/Windowing
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef UWINDOW_H
#define UWINDOW_H

#include "UniversalUI/Base/Geometry.h"
#include "UniversalUI/Window/uWindowHandle.h"


class uWindow {
public:

    uWindowHandle handle;
    uSize size;

    int displayWidth;
    int displayHeight;

    uWindow(const char* title, uSize size);

    void PollEvents();

    virtual void Resized(uSize newSize);
    virtual void Render();

private:

};




#endif // UWINDOW_H