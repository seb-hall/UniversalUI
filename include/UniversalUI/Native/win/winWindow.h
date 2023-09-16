//  winWindow.h - UniversalUI/Native/win
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifdef _WIN32 // ignore file if not on windows

#define UNICODE

#ifndef WINWINDOW_H
#define WINWINDOW_H


#include "UniversalUI/Core/Geometry.h"
#include "UniversalUI/Windowing/uWindow.h"
#include "UniversalUI/Native/nWindow.h"











namespace UniversalUI {

    class winWindow : public nWindow {
        
        HWND windowHandle;


        public:

        winWindow() {

            

            
        }

        void Show() override {
            ShowWindow(windowHandle, true);
        }

    };


}

#endif // WINWINDOW_H
#endif // _WIN32