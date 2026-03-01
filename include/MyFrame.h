/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   MyMainFrame.h
  * Author: petrstepanov
  *
  * Created on October 15, 2017, 1:47 AM
  */

#ifndef MYFRAME_H
#define MYFRAME_H

#include<TGFrame.h>
#include<TXMLEngine.h>
#include<TGLabel.h>
#include<TGButton.h>
#include<TG3DLine.h>
#include<TRootEmbeddedCanvas.h>

class MyFrame : public TGVerticalFrame {
public:
    MyFrame(const TGWindow* p = 0);
    virtual ~MyFrame();

    // UI elements
    TGLabel* pathLabel;
    TGTextButton* pathButton;
    TGTextButton* plotButton;
    TGHorizontalFrame* hFrame;
    TGHorizontal3DLine* hLine;
    TRootEmbeddedCanvas* embedCanvas;
    TGHorizontal3DLine* hLine2;
    TGHorizontalFrame* hFrame2;
    TGTextButton* exitButton;
    TGTextButton* saveButton;

    struct XYPoints {
        double x[11];
        double y[11];
    };

    // Variables
	TString configPath;
    std::map<int, TString> lensIdMap;
    std::map<TString, int> laserMarkerStyles;
    int myMarkerStyles[15] = {2, 5, 3, 25, 24, 20, 4, 21, 22, 30, 23, 29, 26, 33, 34};
    std::map<int, std::map<TString, XYPoints>> laserDataMap;

    // Methods
    void clearCanvas();
    int processLensXML();
    int processLaserXML();

    // SLOTS
    void onPathButtonClicked();
    void onPlotButtonClicked();
    void onExitButtonClicked();
    void onSaveButtonClicked();

    ClassDef(MyFrame, 0);
};

#endif /* MyFrameW_H */
