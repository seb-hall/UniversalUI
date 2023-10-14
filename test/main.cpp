//  main.cpp - UniversalUI/test
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#include "UniversalUI/UniversalUI.h"


#include <stdio.h>



int main() {

    uApplication* app = new uApplication;

    return UniversalUI(app);
}