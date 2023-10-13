//  UniversalUI.h - UniversalUI
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef UNIVERSALUI_H
#define UNIVERSALUI_H

#include "UniversalUI/Window/uWindow.h"
#include "UniversalUI/Application/uApplication.h"
#include "UniversalUI/Misc/Types.h"

#include "OSC/oscSubmodule.h"

#include <stdio.h>
#include <vector>

class UniversalUI : public oscSubmodule {

public: 

	UniversalUI() {
		GUID = "org.UniversalUI.main";
		name = "UniversalUI";
		developer = "Seb Hall";
	}

	//	main lifecycle events
	bool Init() override {
		printf("UniversalUI init\n");
		return true;
	}

	void Shutdown() override {
		printf("UniversalUI shutdown\n");
	}

	//	message events
	std::vector<oscMessage> SendOutgoingMessages() override {
		return {};
	}
	void RecieveIncomingMessages(std::vector<oscMessage> incomingMessages) override {

	}

};


extern "C" __declspec(dllexport) oscSubmodule* GetSubmodule() {
	UniversalUI* toReturn = new UniversalUI;
	//printf("made new toReturn");
	return toReturn;
}

//int UniversalUI(uApplication* app);


#endif // UNIVERSALUI_H