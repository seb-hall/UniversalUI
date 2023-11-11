//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOWMANAGER_H
#define UWINDOWMANAGER_H

#include <vector>

#include "uWindow.h"

class uWindowManager {

    static std::vector<uWindow*> windows;

public:

    static bool Init();

    static void PollEvents();

    static bool IsWindowsEmpty();

    static void CreateNewWindow(uWindow* window, double width, double height, std::string title);
    static void SetWindowVisibility(uWindow* window, uWindowVisibility visibility);

    //  for now used for win32 processing
    static void DestroyWindowByHandle(uWindowHandle handle);
};

#endif //UWINDOWMANAGER_H
