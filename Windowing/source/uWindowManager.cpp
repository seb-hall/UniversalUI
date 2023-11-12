//
// Created by sebam on 11/11/2023.
//

#include "../include/uWindowManager.h"

#include "uRenderManager.h"

std::vector<uWindow*> uWindowManager::windows;

#include <cstdio>

#ifdef _WIN32

const wchar_t CLASS_NAME[]  = L"UniversalUI Window";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif


bool uWindowManager::Init() {

#ifdef _WIN32

    // Register the window class.
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandleW(nullptr);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == 0) {
        return false;
    }

#endif

    return true;
}


void uWindowManager::PollEvents() {
    #ifdef _WIN32
        // Run the message loop.
        MSG msg = { };
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    #endif
}

bool uWindowManager::IsWindowsEmpty() {
    return windows.empty();
}

void uWindowManager::CreateNewWindow(uWindow* window, double width, double height, std::string title) {

    #ifdef _WIN32
        std::wstring wideTitle = std::wstring(title.begin(), title.end());

        window->systemHandle = CreateWindowExW(
                0,                              // Optional window styles.
                CLASS_NAME,                     // Window class
                wideTitle.c_str(),  // Window text
                WS_OVERLAPPEDWINDOW,            // Window style

                // Size and position
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,


                nullptr,       // Parent window
                nullptr,       // Menu
                GetModuleHandleW(nullptr),  // Instance handle
                nullptr        // Additional application data
        );

        if (window->systemHandle == nullptr)
        {
            printf("ERROR: Window creation failed!");
            return;
        }

        uRenderManager::SetupForWindow(window);
        uRenderManager::RenderToWindow(window);

        windows.push_back(window);
    #endif // _WIN32

}

void uWindowManager::DestroyWindowByHandle(uWindowHandle handle) {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i]->systemHandle == handle) {

            #ifdef _WIN32
                if (DestroyWindow(handle)) {
                    windows.erase(windows.begin() + i);
                    i--;
                }
            #endif // _WIN32
        }
    }
}

void uWindowManager::SetWindowVisibility(uWindow* window, uWindowVisibility visibility) {

    #ifdef _WIN32
        switch (visibility) {
            case uWindowVisibility::VISIBLE:
                ShowWindow(window->systemHandle, SW_SHOW);
                break;
            case uWindowVisibility::MINIMISED:
                ShowWindow(window->systemHandle, SW_SHOWMINIMIZED);
                break;
            default:
                ShowWindow(window->systemHandle, SW_HIDE);
                break;
        }
    #endif // _WIN32
}



#ifdef _WIN32

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            uWindowManager::DestroyWindowByHandle(hwnd);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
        }
            return 0;
        default:
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif
