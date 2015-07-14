#pragma once;

#include "ofxValuePlotter.h"

class ofxFpsDisplay: public ofxValuePlotter {
public:
    ofxFpsDisplay();
    ofxFpsDisplay(float minValue, float maxValue, int plotSize=100, float width = defaultWidth, float height = defaultHeight);
    virtual ~ofxFpsDisplay();

    void update();

    ofxFpsDisplay * setup(float minValue=0, float maxValue=0, int plotSize=100, float width = defaultWidth, float height = defaultHeight);

protected:

};
