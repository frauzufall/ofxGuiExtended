#include "ofxGuiFpsPlotter.h"
#include "ofAppRunner.h"

ofxGuiFpsPlotter::ofxGuiFpsPlotter()
:ofxGuiValuePlotter(){

	setName("FPS");
	setup();

}

ofxGuiFpsPlotter::ofxGuiFpsPlotter(const ofJson & config)
:ofxGuiFpsPlotter(){

	_setConfig(config);

}

ofxGuiFpsPlotter::ofxGuiFpsPlotter(std::string label, float minValue, float maxValue, int plotSize, const ofJson &config)
	:ofxGuiValuePlotter(label, minValue, maxValue, plotSize, config){

	setup();

}

ofxGuiFpsPlotter::~ofxGuiFpsPlotter(){
	ofRemoveListener(ofEvents().update,this,&ofxGuiFpsPlotter::update);
}

void ofxGuiFpsPlotter::setup(){
	if(minVal.get() == maxVal.get()) {
		if(ofGetTargetFrameRate() > 0) {
			minVal = 0;
			maxVal = ofGetTargetFrameRate();
		}
	}
	setDecimalPlace(0);
	ofAddListener(ofEvents().update,this,&ofxGuiFpsPlotter::update);
}

void ofxGuiFpsPlotter::update(ofEventArgs &){
	value = ofGetFrameRate();
	setNeedsRedraw();
}

