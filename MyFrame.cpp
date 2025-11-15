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
#include <iostream>
#include <TGFileDialog.h>
#include <TGMsgBox.h>
#include <TSystem.h>
#include <TApplication.h>
#include "MyFrame.h"

// Need for SIGNALS/SLOTS below - Connect to exit()
ClassImp(MyFrame);

MyFrame::MyFrame(const TGWindow* p) : TGVerticalFrame(p) {
    // Init UI
    int dx = 15;
    int dy = 10;
	hFrame = new TGHorizontalFrame(this);

    pathLabel = new TGLabel(hFrame, "No Directory Specified");
    pathLabel->SetTextJustify(kTextLeft);
	pathLabel->Set3DStyle(kSunkenFrame);
    hFrame->AddFrame(pathLabel, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 0, dx, 0, 0));

    plotButton = new TGTextButton(hFrame, "Plot");
    hFrame->AddFrame(plotButton, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));
    plotButton->SetEnabled(kFALSE);

    pathButton = new TGTextButton(hFrame, "Set SemiShop Folder...");
    hFrame->AddFrame(pathButton, new TGLayoutHints(kLHintsRight | kLHintsTop, dx, dx, 0, 0));

	AddFrame(hFrame, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 0, 0, 0, dy));

    hLine = new TGHorizontal3DLine(this);
    AddFrame(hLine, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));

    embedCanvas = new TRootEmbeddedCanvas("embedHist", this, 10, 10, EFrameType::kChildFrame); // EFrameType::kChildFrame - no border
    AddFrame(embedCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, dy, 0));

    hLine2 = new TGHorizontal3DLine(this);
    AddFrame(hLine2, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));

    hFrame2 = new TGHorizontalFrame(this);

    exitButton = new TGTextButton(hFrame2, "Exit");
    hFrame2->AddFrame(exitButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));

    saveButton = new TGTextButton(hFrame2, "Save");
    hFrame2->AddFrame(saveButton, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));
    saveButton->SetEnabled(kFALSE);

    AddFrame(hFrame2, new TGLayoutHints(kLHintsExpandX | kLHintsBottom, 0, 0, dy, 0));

    // Events
    pathButton->Connect("Clicked()", "MyFrame", this, "onPathButtonClicked()");
    plotButton->Connect("Clicked()", "MyFrame", this, "onPlotButtonClicked()");
    exitButton->Connect("Clicked()", "MyFrame", this, "onExitButtonClicked()");
    saveButton->Connect("Clicked()", "MyFrame", this, "onSaveButtonClicked()");
}

MyFrame::~MyFrame() {
    delete pathLabel;
    delete pathButton;
    delete hFrame;
    delete embedCanvas;
}

void MyFrame::clearCanvas() {
    embedCanvas->Clear();
}

void MyFrame::onPathButtonClicked() {
    const char* filetypes[] = { "All files", "*", "ROOT files", "*.root", 0, 0 };
    TGFileInfo fi;
    fi.fFileTypes = filetypes;
    fi.SetMultipleSelection(kFALSE);
    TGFileDialog* fileDialog = new TGFileDialog(gClient->GetRoot(), this, kDOpen, &fi);

    // Error - multiple selected
    if (fi.fMultipleSelection) {
        TGMsgBox* msgBox = new TGMsgBox(gClient->GetRoot(), this, "Error", "Cannot select multiple directories", EMsgBoxIcon::kMBIconStop, EMsgBoxButton::kMBOk);
        plotButton->SetEnabled(kFALSE);
        // delete msgBox;
        // delete fileDialog;
        return;
    }
    // Error - dir not exist
    if (gSystem->AccessPathName(fi.fFilename, EAccessMode::kFileExists)){
        TGMsgBox* msgBox = new TGMsgBox(gClient->GetRoot(), this, "Error", "Directory does not exist", EMsgBoxIcon::kMBIconStop, EMsgBoxButton::kMBOk);
        plotButton->SetEnabled(kFALSE);
        // delete msgBox;
        // delete fileDialog;
        return;
    }
    plotButton->SetEnabled(kTRUE);
    pathLabel->SetText(fi.fFilename);
    configPath = TString::Format(fi.fFilename);
    lensIdMap.clear();
    // delete fileDialog;
}

void MyFrame::onPlotButtonClicked() {
    TString filename = configPath;
    filename += "/";
    filename += "Configuration";
    filename += "/";
    filename += "Common";
    filename += "/";
    filename += "System";
    filename += "/";
    filename += "Lens.setting.xml";


    std::cout << filename << std::endl;

    // First create engine
    TXMLEngine* xml = new TXMLEngine();

    // Now try to parse xml file
    // Only file with restricted xml syntax are supported
    XMLDocPointer_t xmldoc = xml->ParseFile(filename);
    if (xmldoc == 0) {
        delete xml;
        return;
    }
    // take access to main node
    XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc); // <LensSetting>
    XMLNodePointer_t child1 = xml->GetChild(mainnode);            // <Turret>, <Option>, <Lens>, <Lens>, <Lens>...
    while (child1 != 0) {
        ProcessLensNode(xml, child1, 1);
		child1 = xml->GetNext(child1);
    }

    // Release memory before exit
    xml->FreeDoc(xmldoc);
    delete xml;
}

void MyFrame::ProcessLensNode(TXMLEngine* xml, XMLNodePointer_t node, Int_t level){
    // this function display all accessible information about xml node and its children
    printf("%*c node: %s\n", level, ' ', xml->GetNodeName(node));
    if (strcmp(xml->GetNodeName(node), "Lens") == 0) {
        TString lensID = xml->GetAttr(node, "LensID");
		Int_t lensIDInt = lensID.Atoi();
        if (lensIdMap.find(lensIDInt) == lensIdMap.end()) {
            XMLNodePointer_t nodeMag = xml->GetChild(node);
            TString lensMag = xml->GetNodeContent(nodeMag);
            lensMag += "x";
			lensIdMap[lensIDInt] = lensMag;
            std::cout << "LensID: " << lensIDInt << " Magnification: " << lensMag << std::endl;
        }
    }
}

void MyFrame::onExitButtonClicked() {
    gApplication->Terminate();
}