#include "ofxFpsDisplay.h"
#include "ofAppRunner.h"

ofxFpsDisplay::ofxFpsDisplay() {}

ofxFpsDisplay::ofxFpsDisplay(float minValue, float maxValue, int plotSize, float width, float height){
    setup(minValue, maxValue, plotSize, width, height);
}

ofxFpsDisplay::~ofxFpsDisplay(){
}

ofxFpsDisplay* ofxFpsDisplay::setup(float minValue, float maxValue, int plotSize, float width, float height) {
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

void ofxFpsDisplay::update() {
    ofxValuePlotter::update(ofGetFrameRate());
}

