#include "ofxFpsPlotter.h"
#include "ofAppRunner.h"

ofxFpsPlotter::ofxFpsPlotter() {}

ofxFpsPlotter::ofxFpsPlotter(float minValue, float maxValue, int plotSize, float width, float height){
    setup(minValue, maxValue, plotSize, width, height);
}

ofxFpsPlotter::~ofxFpsPlotter(){
}

ofxFpsPlotter* ofxFpsPlotter::setup(float minValue, float maxValue, int plotSize, float width, float height) {
    if(minValue == maxValue) {
        if(ofGetTargetFrameRate() > 0) {
            minValue = 0;
            maxValue = ofGetTargetFrameRate();
        }
    }
    ofxValuePlotter::setup("FPS", minValue, maxValue, plotSize, width, height);
    setDecimalPlace(0);
    return this;
}

void ofxFpsPlotter::update() {
    ofxValuePlotter::update(ofGetFrameRate());
}

