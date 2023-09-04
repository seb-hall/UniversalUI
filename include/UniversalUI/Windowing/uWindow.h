//  uWindow.h - UniversalUI/Windowing
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef UWINDOW_H
#define UWINDOW_H

#include "UniversalUI/Core/Geometry.h"

namespace UniversalUI {

    class uWindow {

    public:

        uRect frame;

        virtual void Show() {
            
        }
    };


}

#endif // UWINDOW_H