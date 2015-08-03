#pragma once

#include "ofxValuePlotter.h"

class ofxFpsPlotter: public ofxValuePlotter {
public:
    ofxFpsPlotter(const ofxValuePlotter::Config & c);
    virtual ~ofxFpsPlotter();
protected:
    void update(ofEventArgs &);

};
