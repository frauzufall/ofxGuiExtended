#pragma once

#include "ofxBaseGui.h"

class ofxValuePlotter: public ofxBaseGui {
public:
    ofxValuePlotter();
    ofxValuePlotter(string label, float minValue=0, float maxValue=0, int plotSize=100, float width = defaultWidth, float height = defaultHeight);
    virtual ~ofxValuePlotter();

    ofxValuePlotter * setup(string label="", float minValue=0, float maxValue=0, int plotSize=100, float width = defaultWidth, float height = defaultHeight);

    void update(float value);
    void setDecimalPlace(int place);

    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
    virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

    virtual ofAbstractParameter & getParameter();

protected:

    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    void render();
    void generateDraw();
    ofPath bg;
    ofVboMesh textMesh;
    vector<float> buffer;
    int plotSize;
    ofPath plot;
    float lastVal;
    float minVal, maxVal;
    bool autoscale;
    int decimalPlace=3;
    ofParameter<string> label;

};
