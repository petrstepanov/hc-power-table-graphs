// RootTest.cpp : Defines the entry point for the application.
//

#include <TApplication.h>
#include <iostream>
#include "MyMainFrame.h"

/*
TString getDirectoryPath() {
    // Set file picker defaults
    const char* filetypes[] = { "All directories", "*", 0, 0 };
    TGFileInfo fi;
    fi.fFileTypes = filetypes;

	TGFileDialog dilaog(gClient->GetRoot(), 0, kDOpen, &fi);
	TGDiec
    if (fi.fFilename) {
        TString fileName(fi.fFilename);
        return fileName;
    }

	// Return empty string if canceled
	TString emptyString("");
	return emptyString;
}
*/

int main(int argc, char** argv) {
	// Start application (for GUI)
	TApplication app("myApp", &argc, argv);

	// Instantiate main frame
	MyMainFrame* myMainFrame = new MyMainFrame();
	// Run simple canvas example
	// TCanvas* canvas= new TCanvas("MyCanvas", "A Simple Canvas", 800, 600);
	// TH1I* hist = new TH1I("hist", "A Simple Histogram", 100, -4, 4);
	// hist->FillRandom("gaus", 10000);
	// hist->Draw();

	// Update the canvas to ensure the drawing is visible.
	// canvas->Modified();
	// canvas->Update();
	// canvas->SaveAs("canvas.png");

	// Open XML file
	// TString path = getDirectoryPath();
	// std::cout << path.Data() << std::endl;

	// Run application loop
	app.Run();

	// Free up memory
	// delete hist;
	// delete canvas;

	// Return success
	return 0;
}
