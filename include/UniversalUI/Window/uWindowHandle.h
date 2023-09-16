//  uWindowHandle.h - UniversalUI/Windowing
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef UWINDOWHANDLE_H
#define UWINDOWHANDLE_H

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

struct uWindowHandle {

#ifdef _WIN32
	HWND rawHandle;
#endif // _WIN32


};



#endif // UWINDOWHANDLE_H