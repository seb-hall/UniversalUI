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

#include <Windows.h>


PCWSTR className = L"UniversalUI Window";


static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

        return DefWindowProc(hwnd, uMsg, wParam, lParam);

}



namespace UniversalUI {

    class winWindow : public nWindow {
        
        HWND windowHandle;


        public:

        winWindow() {

            WNDCLASS wc = {0};

            wc.lpfnWndProc   = WindowProc;
            wc.hInstance     = GetModuleHandle(NULL);
            wc.lpszClassName = className;

            RegisterClass(&wc);

            windowHandle = CreateWindowEx(
                0,                              // Optional window styles.
                className,                     // Window class
                L"Learn to Program Windows",    // Window text
                WS_OVERLAPPEDWINDOW,            // Window style

                // Size and position
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                NULL,       // Parent window    
                NULL,       // Menu
                GetModuleHandle(NULL),  // Instance handle
                NULL        // Additional application data
            );  
        }

        void Show() override {
            ShowWindow(windowHandle, true);
        }

    };


}

#endif // WINWINDOW_H
#endif // _WIN32