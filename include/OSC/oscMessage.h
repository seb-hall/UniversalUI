//	oscMessage.h - UniversalSDK OSC
//	First written by Seb Hall on 17/09/2023
//
//	UniversalSDK is a modular and extendable 
//	software development framework intended
//	for building high-performance, cross-platform
//	applications.
//
//	oscMessage provides a mechanism for submodules
//	to communicate with each other.

#ifndef OSCMESSAGE_H
#define OSCMESSAGE_H

#include <vector>

struct oscMessage {

	//	info for osc message sender
	const char* sender;
	const char* reciever;

	int messageID = 0;
	int ttlMs = 50;

	std::vector<char> rawBytes;

};

#endif // OSCMESSAGE_H