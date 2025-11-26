// RootTest.cpp : Defines the entry point for the application.
//

#include "TApplication.h"
#include <iostream>
#include "MyMainFrame.h"

int main(int argc, char** argv) {
	// Start application (for GUI)
	TApplication app("myApp", &argc, argv);

	// Instantiate main application frame
	MyMainFrame* myMainFrame = new MyMainFrame();

	// Run application event loop
	app.Run();

	// Return success
	return 0;
}
