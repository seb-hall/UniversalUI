//  main.cpp - UniversalUI/test
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#include "UniversalUI/UniversalUI.h"


#include <stdio.h>

class TestApp : public uApplication {

public:

    TestApp() {
        title = "TestApp";
        developer = "Test Developer";
    }

    void ApplicationLaunched() override {
        printf("TEST APP LAUNCHED\n");

        uWindow* window1 = new uWindow("my window 1", {800.0f, 600.0f});
        window1->forceSmoothResize = true;
        windows.push_back(window1);

        uWindow* window2 = new uWindow("my window 2", {800.0f, 600.0f});
        window2->forceSmoothResize = false;
        windows.push_back(window2);

        
    }

};

int main() {

    return UniversalUI(new TestApp());
}