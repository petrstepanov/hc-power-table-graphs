/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   MyMainFrame.cpp
  * Author: petrstepanov
  *
  * Created on October 15, 2017, 1:47 AM
  */

// #include "TImage.h"
#include <TApplication.h>
#include <TGClient.h>
#include <iostream>
#include "MyMainFrame.h"

#include "PowerGraphs.xpm"

// Need for SIGNALS/SLOTS below - Connect to exit()
ClassImp(MyMainFrame);

MyMainFrame::MyMainFrame() : TGMainFrame(gClient->GetRoot(), 1024, 600) {
    // For the X window system
    SetWindowName("Laser Power Table Plots");

    // This I am not sure if we need this? Does not seem to do anything, however let it be here.
    SetIconName("PowerGraphs");

    // In GNOME, SetClassHints() tells WM to pick application icon installed on the system
    SetClassHints("PowerGraphs", "PowerGraphs");

    // Icon how-to:
    // * Design 256x256 .png
    // * In GIMP export to .xpm
    // * Include .xpm with #include directive above
    // * Ensure "const char* []" is in .xpm (not static)
    // * Convert const char* to char** by droppiong const:
    //   https://stackoverflow.com/questions/833034/how-to-convert-const-char-to-char
    SetIconPixmap(const_cast<char **>(power_graphs_xpm));

    SetCleanup(kDeepCleanup);

    // Exit this application via the Exit button or Window Manager.
    Connect("CloseWindow()", "MyMainFrame", this, "exit()");
    DontCallClose();

    // Add child TGFrame
    myFrame = new MyFrame(this);
    AddFrame(myFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 15, 15, 10, 10));

    // Set minimum and maximum window size for window manager
    // SetWMSizeHints(Constants::windowWidth, Constants::windowHeight, 1E4, 1E4, 1, 1);
    MapSubwindows();
    Layout();

    // Issue - GetDefaultSize() returns zeros on Ubuntu 18.04.
    // Resize(Constants::windowWidth, Constants::windowHeight);
    // Maps the window "id" and all of its subwindows that have had map requests on the screen
    // and put this window on the top of of the stack of all windows
    MapWindow();

    // Change all sub frame colors to gray
    ChangeSubframesBackground(15263976);
}

MyMainFrame::~MyMainFrame() {
    Cleanup();
    delete myFrame;
}

void MyMainFrame::exit() {
    gApplication->Terminate();   // to exit and close the ROOT session
}
