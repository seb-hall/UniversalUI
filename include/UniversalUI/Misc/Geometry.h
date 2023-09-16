//  Geometry.h - UniversalUI/Core
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "UniversalUI/Misc/Types.h"

struct uPoint {
    f32 x;
    f32 y;
};

struct uSize {
    f32 width;
    f32 height;
};

struct uRect {
    f32 x;
    f32 y;
    f32 width;
    f32 height;
};



#endif // GEOMETRY_H