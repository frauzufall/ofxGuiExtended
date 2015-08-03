#include "ofxFpsPlotter.h"
#include "ofAppRunner.h"

ofxFpsPlotter::ofxFpsPlotter(const ofxValuePlotter::Config & c)
:ofxValuePlotter(ofParameter<float>("fps",0),c){
    if(minVal == maxVal) {
        if(ofGetTargetFrameRate() > 0) {
        	minVal = 0;
        	maxVal = ofGetTargetFrameRate();
        }
    }
    setDecimalPlace(0);
    ofAddListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

ofxFpsPlotter::~ofxFpsPlotter(){
	ofRemoveListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

void ofxFpsPlotter::update(ofEventArgs &) {
    value = ofGetFrameRate();
}

