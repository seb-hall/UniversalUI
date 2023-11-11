//
// Created by sebam on 11/1/2023.
//

#include <stdio.h>

#include "UniversalUI.h"
#include "uApplication.h"
#include "uWindow.h"

class MyApp : public uApplication {

    void FinishedLaunching() override {

        uWindow* window1 = new uWindow(1000.0, 750.0, "window 1");
        window1->SetVisibility(uWindowVisibility::VISIBLE);

        uWindow* window2 = new uWindow(1000.0, 750.0, "window 2");
        window2->SetVisibility(uWindowVisibility::MINIMISED);

        printf("finished launching!");
    }
};



int main() {

    MyApp* app = new MyApp();
    return UniversalUI(app);
}