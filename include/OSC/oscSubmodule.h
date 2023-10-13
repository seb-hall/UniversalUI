//	oscSubmodule.h - UniversalSDK OSC
//	First written by Seb Hall on 17/09/2023
//
//	UniversalSDK is a modular and extendable 
//	software development framework intended
//	for building high-performance, cross-platform
//	applications.
//
//	oscSubmodule provides a base class for all
//	submodules.

#ifndef OSCSUBMODULE_H
#define OSCSUBMODULE_H

#include "OSC/oscMessage.h"
#include <vector>

class oscSubmodule {

public:

	int majorVersion;
	int minorVersion;

	const char* GUID;
	const char* name;
	const char* developer;

	bool isLocal;

	size_t rawHandle;

	//	main lifecycle events
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;

	//	message events
	virtual std::vector<oscMessage> SendOutgoingMessages() = 0;
	virtual void RecieveIncomingMessages(std::vector<oscMessage> incomingMessages) = 0;

};


#endif // OSCSUBMODULE_H