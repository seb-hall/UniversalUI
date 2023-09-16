#include "UniversalUI/Application/uApplication.h"

void uApplication::ApplicationLaunched() {
	title = "UniversalUI Application";
	developer = "Unknown Developer";
}

void uApplication::ApplicationWillQuit() {

}

bool uApplication::ApplicationShouldQuit() {
	return true;
}

int uApplication::Run() {
	while (windows.size() > 0) {
		for (uWindow* window : windows) {
			window->PollEvents();
		}
	}

	return 0;
}