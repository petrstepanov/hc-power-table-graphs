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
#include <TCanvas.h>
#include <TMultiGraph.h>
#include <TTimeStamp.h>
#include <DateHelper.h>
#include <TAxis.h>
#include <TText.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TVirtualPad.h>
#include "CanvasHelper.h"
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

    // Add embedded canvas and set its frame to Sunken
    // https://root.cern/doc/master/TGFrame_8cxx_source.html#l00360
    embedCanvas = new TRootEmbeddedCanvas("embedHist", this, 10, 10, EFrameType::kChildFrame); // EFrameType::kChildFrame - no border
    // embedCanvas->GetCanvas()->SetEditable(kFALSE);
    // embedCanvas->GetCanvas()->SetBit(kNoContextMenu);
    // embedCanvas->GetCanvas()->SetFillColor(kGray-1);

    embedCanvas->GetCanvas()->cd();
    TText t;
    t.SetTextSize(0.025);
    t.SetTextAlign(kHAlignCenter + kVAlignCenter);
    t.DrawText(0.5, 0.5, "Please Specify SemiShop Root Folder");

    AddFrame(embedCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, dy, 0));

    hLine2 = new TGHorizontal3DLine(this);
    AddFrame(hLine2, new TGLayoutHints(kLHintsExpandX, 0, 0, dy, 0));

    hFrame2 = new TGHorizontalFrame(this);

    exitButton = new TGTextButton(hFrame2, "Exit");
    hFrame2->AddFrame(exitButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));

    saveButton = new TGTextButton(hFrame2, "Save Image...");
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

void MyFrame::onSaveButtonClicked() {
    TString filename = configPath;
    filename += "/Configuration/Common/System/LaserPower.setting.xml";
    FileStat_t fs;
    gSystem->GetPathInfo(filename.Data(), fs);
    TString workDir = gSystem->GetWorkingDirectory();
    TString fileName = DateHelper::formatTimeStamp(fs.fMtime);
    fileName += ".png";
    TString pathName = gSystem->ConcatFileName(workDir, fileName);
    embedCanvas->SaveAs(pathName);

    // Message box success
    TString text = TString::Format("Image \"%s\" successfully saved.", pathName.Data());
    TGMsgBox* box = new TGMsgBox(gClient->GetRoot(), this, "Success", text.Data(), EMsgBoxIcon::kMBIconAsterisk, EMsgBoxButton::kMBOk);
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
    // Error - Cancel button clicked
    if (fi.fFilename == nullptr){
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
    // delete fileDialog;
}

void MyFrame::onPlotButtonClicked() {
    // Get NIR lens map
    processLensXML();

    // Get power data points
    processLaserXML();

    // Prepare canvas
    Int_t nCanvases = lensIdMap.size();
    TCanvas* canvas = embedCanvas->GetCanvas();
    canvas->Clear();

    // Split canvases and leave no space in between , 1E-5, 1E-5);
    switch (nCanvases){
        case 2:
            canvas->Divide(nCanvases, 1, 1E-5, 1E-5);
            break;
        case 3:
            canvas->Divide(nCanvases, 1, 1E-5, 1E-5);
            break;
        case 4:
            canvas->Divide(2, 2, 1E-5, 1E-5);
            break;
        case 5:
            canvas->Divide(3, 2, 1E-5, 1E-5);
            break;
        case 6:
            canvas->Divide(3, 2, 1E-5, 1E-5);
            break;
        default:
            canvas->Divide(3, 2, 1E-5, 1E-5);
            break;
    }
    int canvasIndex = 1;

    // Iterate over lens map
    for (auto const& [lensIdInt, lensName] : lensIdMap){
        // Create multi-graph
        TMultiGraph* mg = new TMultiGraph();

        // Iterate over each laser for given lens and create graphs
        std::map<TString, XYPoints> laserDataMapForLens = laserDataMap[lensIdInt];

        // Remove zero points and points 1...10
        std::map<TString, XYPoints>::iterator it;
        // TRICK: Remove pairs while iterating the map
        // https://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it
        for (it = laserDataMapForLens.begin(); it != laserDataMapForLens.end(); /*it++*/){
            auto laserName = it->first;
            auto xyPoints = it->second;

            // Populate data points
            Bool_t allPointsZero = kTRUE;   // To exclude power values all 0
            Bool_t allPoints1To10 = kTRUE;  // To exclude power values Semishop default 1,2,3,4,54,6,7,8,9,10
            for (int i=0; i < 11; i++){
                double x = xyPoints.x[i];
                double y = xyPoints.y[i];
                if (y != 0) allPointsZero = kFALSE;
                if (y != i) allPoints1To10 = kFALSE;
            }

            // If data is empty - skip graph
            if (allPointsZero || allPoints1To10){
                laserDataMapForLens.erase(it++);
            }
            else {
                ++it;
            }
        }

        // Draw multi-graph on the canvas
        TVirtualPad* pad = canvas->cd(canvasIndex);

        // Iterate "useful" graph data
        std::map<TString, XYPoints>::iterator it2;
        for (it2 = laserDataMapForLens.begin(); it2 != laserDataMapForLens.end(); it2++){
            // for (auto const& [laserName, xyPoints] : laserDataMapForLens){
            auto laserName = it2->first;
            auto xyPoints = it2->second;
            // Create new graph
            TGraph* graph = new TGraph();
            TString title = lensName;
            title += "x ";
            title += laserName;

            // Populate data points
            // Bool_t allPointsZero = kTRUE;   // Power values are all 0
            // Bool_t allPoints1To10 = kTRUE;  // Power values are default 1,2,3,4,54,6,7,8,9,10
            for (int i=0; i < 11; i++){
                double x = xyPoints.x[i];
                double y = xyPoints.y[i];
                graph->AddPoint(x, y);
                std::cout << title << " " << x << " " << y << std::endl;
            }

            // Visual styling
            graph->SetTitle(title);
            graph->SetMarkerStyle(laserMarkerStyles[laserName]);


            mg->Add(graph);
            // Hack - see ROOT's TMultigraph example - draw last graph, not add. Otherwise legend issue
            // https://root.cern.ch/doc/master/classTMultiGraph.html
            // auto lastPair = std::prev(laserDataMapForLens.end());
            // if (it2 != lastPair){
            //     mg->Add(graph);
            // }
            // else {
            //     graph->Draw("ALP");
            // }
        }

        // TString title = lensName;
        TString title = ";Laser Intensity, %;Power Delivered to Sample, mW";
        mg->SetTitle(title);
        mg->Draw("ALP");

        // ROOT is terrible. Need to do things in "its" right order
        mg->GetXaxis()->SetLimits(0, 100);
        mg->GetXaxis()->CenterTitle(true);
        mg->GetYaxis()->CenterTitle(true);

        // Build and align legend
        // Cannot do it here because legend creation interferes with other primitives!
        // Solution - build Legend very small in top left corner - so it does not interfere
        // pad->BuildLegend();
        pad->BuildLegend(0.01, 0.99, 0.02, 1);


        // Set grid after legend was generated to reduce interference
        pad->SetGrid();

        // Now we align the legend that it was added - via CanvasHelper
        TLegend *legend = CanvasHelper::getDefaultLegend(pad);
        CanvasHelper::setPaveAlignment(legend, kPaveAlignLeft | kPaveAlignTop);

        // Remember switch to next canvas
        canvasIndex++;
    }

    // Add multi-title with modified date
    TString title = "Laser Power Table Graphs ";

    // Add laser powers file Modification Date to the title
    TString filename = configPath;
    filename += "/Configuration/Common/System/LaserPower.setting.xml";
    FileStat_t fs;
    gSystem->GetPathInfo(filename.Data(), fs);
    title += DateHelper::formatTimeStamp(fs.fMtime);
    CanvasHelper::addMultiCanvasTitle(canvas, title);

    // Tweak canvas
    CanvasHelper::getInstance()->addCanvas(canvas);

    // Enable Save button
    saveButton->SetEnabled(kTRUE);
}

int MyFrame::processLensXML() {
    TString filename = configPath;
    filename += "/";
    filename += "Configuration";
    filename += "/";
    filename += "Common";
    filename += "/";
    filename += "System";
    filename += "/";
    filename += "Lens.setting.xml";

    // Check if file exists
    if (gSystem->AccessPathName(filename, EAccessMode::kFileExists) == kTRUE) return 1;

    // First create engine
    TXMLEngine* xml = new TXMLEngine();

    // Now try to parse xml file
    // Only file with restricted xml syntax are supported
    XMLDocPointer_t xmldoc = xml->ParseFile(filename);
    if (xmldoc == 0) {
        delete xml;
        return 2;
    }
    // take access to main node
    XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc); // <LensSetting>
    XMLNodePointer_t node = xml->GetChild(mainnode);      // <Turret>, <Option>, <Lens>, <Lens>, <Lens>...

    // Clear existing map
    lensIdMap.clear();

    // Process "Lens" nodes
    while (node != 0) {
        // Ensure the node is <Lens> - skip <Turret> and <Option>
        if (strcmp(xml->GetNodeName(node), "Lens") != 0){
            node = xml->GetNext(node);
            continue;
        }

        // Get LensID attribute
        TString lensID = xml->GetAttr(node, "LensID");
        Int_t lensIDInt = lensID.Atoi();

        // Ensure this lensID is not added to the map already
        if (lensIdMap.find(lensIDInt) != lensIdMap.end()){
            node = xml->GetNext(node);
            continue;
        }

        // Obtain lens magnification
        XMLNodePointer_t nodeMag = xml->GetChild(node);
        TString lensMag = xml->GetNodeContent(nodeMag);

        // Obtain lens wavelength
        XMLNodePointer_t nodeWl = xml->GetNext(nodeMag);
        TString lensWl = xml->GetNodeContent(nodeWl);

        // Filter out 0.01x lens
        if (lensMag == "0.01"){
            node = xml->GetNext(node);
            continue;
        }

        // Ensure wavelength is NIR or VIS (20x, 50x, 100x)
        if (!(lensWl == "NIR" || lensWl == "VIS")){
            node = xml->GetNext(node);
            continue;
        }

        // Add lens entry to the map
        lensIdMap[lensIDInt] = lensMag;

        // Move to next node
        node = xml->GetNext(node);
    }

    // Release memory before exit
    xml->FreeDoc(xmldoc);
    delete xml;
    return 0;
}

int MyFrame::processLaserXML() {
    TString filename = configPath;
    filename += "/";
    filename += "Configuration";
    filename += "/";
    filename += "Common";
    filename += "/";
    filename += "System";
    filename += "/";
    filename += "LaserPower.setting.xml";

    // Clear existing map
    laserDataMap.clear();

    // Check if file exists
    if (gSystem->AccessPathName(filename, EAccessMode::kFileExists) == kTRUE) return 1;

    // First create engine
    TXMLEngine* xml = new TXMLEngine();

    // Now try to parse xml file
    // Only file with restricted xml syntax are supported
    XMLDocPointer_t xmldoc = xml->ParseFile(filename);
    if (xmldoc == 0) {
        delete xml;
        return 2;
    }
    // take access to main node
    XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc); // <LaserPowerSettingParameter>
    XMLNodePointer_t child1 = xml->GetChild(mainnode);          // <Powers>
    XMLNodePointer_t node = xml->GetChild(child1);            // <Setting>

    Int_t currentLaserMarkerIndex = 0;
    while (node != 0) {
        // Ensure the node is <Lens> - skip <Turret> and <Option>
        if (strcmp(xml->GetNodeName(node), "Setting") != 0){
            node = xml->GetNext(node);
            continue;
        }

        // Get LensID attribute
        Int_t lensIDInt = xml->GetIntAttr(node, "LensID");

        // Get laser type
        TString laserType = xml->GetAttr(node, "LaserType");

        // Get laser percentage
        Int_t laserPercentageInt = xml->GetIntAttr(node, "LaserPowerPercentage");

        // Get laser power node
        XMLNodePointer_t powerNode = xml->GetChild(node); // <LaserPowerWattage
        TString wattage = xml->GetNodeContent(powerNode);
        Double_t wattageDouble = wattage.Atof();

        // Save data in the map
        std::map<TString, XYPoints> laserDataMapForLens = laserDataMap[lensIDInt];
        XYPoints xyPoints = laserDataMapForLens[laserType];
        xyPoints.x[laserPercentageInt/10] = laserPercentageInt;
        xyPoints.y[laserPercentageInt/10] = wattageDouble;
        laserDataMapForLens[laserType] = xyPoints;
        laserDataMap[lensIDInt] = laserDataMapForLens;

        // Remember lasers to assign markers
        if (laserMarkerStyles.find(laserType) == laserMarkerStyles.end()){
            laserMarkerStyles[laserType] = myMarkerStyles[currentLaserMarkerIndex];
            currentLaserMarkerIndex++;
        }
        // Move to next node
        node = xml->GetNext(node);
    }

    // Release memory before exit
    xml->FreeDoc(xmldoc);
    delete xml;
    return 0;
}


void MyFrame::onExitButtonClicked() {
    gApplication->Terminate();
}
