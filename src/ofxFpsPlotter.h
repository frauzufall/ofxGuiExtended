#pragma once

#include "ofxValuePlotter.h"

class ofxFpsPlotter: public ofxValuePlotter {
public:
    ofxFpsPlotter();
    ofxFpsPlotter(float minValue, float maxValue, int plotSize=100, float width = defaultWidth, float height = defaultHeight);
    virtual ~ofxFpsPlotter();

    void update();

    ofxFpsPlotter * setup(float minValue=0, float maxValue=0, int plotSize=100, float width = defaultWidth, float height = defaultHeight);

protected:

};
