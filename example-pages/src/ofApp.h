#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(ofMouseEventArgs &args);
    void mouseDragged(ofMouseEventArgs &args);
    void mousePressed(ofMouseEventArgs &args);
    void mouseReleased(ofMouseEventArgs &args);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:

    ofxGuiPage page;

    ofxTabbedPages tabbed_pages;
    ofxGuiPage page1, page2;

    ofxPanelExtended panel1, panel2, panel3;

    void setupPage1();
    void setupPage2();
    void setupPage3();

    ofxGuiGroupExtended rotary;
    ofxGuiMatrix matrix;
    ofParameter<float> slider_param;

};

