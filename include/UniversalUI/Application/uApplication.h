//  uApplication.h - UniversalUI
//  first created by seb hall on 04/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.

#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include "UniversalUI/Misc/Types.h"
#include "UniversalUI/Window/uWindow.h"

class uApplication {

protected:

	//	Application info
	string title;
	string developer;
	u16 majorVersion;
	u16 minorVersion;

	vector<uWindow*> windows;

	void AddWindow(uWindow* window);

public:

	//	Called when self-tests complete and application
	//	launched ok. In this method you should open windows
	//	as no windows will cause the application to quit.
	virtual void ApplicationLaunched();

	//	Called when the application will (and must) quit
	//	perform any critical file saving etc here.
	virtual void ApplicationWillQuit();

	//	Called when the user has requested the application
	//	to quit. This could be used to stop the application
	//	quitting and show a "please save files" dialogue, for
	//	instance.
	virtual bool ApplicationShouldQuit();

	//	Main events loop
	virtual int Run();

};


#endif // UAPPLICATION_H