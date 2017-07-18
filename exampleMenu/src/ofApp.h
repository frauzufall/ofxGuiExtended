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

	void drawCircle();
	void drawRectangle();

	ofxGui gui;
	ofParameterGroup data;
	ofParameter<void> addCircle, addRectangle;
	ofParameterGroup settings, circleSettings, rectangleSettings;
	ofParameter<float> circleRadius, rectangleWidth, rectangleHeight;
	ofParameter<ofColor> circleColor, rectangleColor;
	//all possible different types creating different interfaces
	ofParameterGroup unusedParameters;
	ofParameter<ofRectangle> rectParameter;
	ofParameter<ofColor> colorParameter;
	ofParameter<ofPoint> pointParameter;
	ofParameter<float> floatParameter;
	ofParameter<std::string> labelParameter;
	ofParameter<bool> boolParameter;
	ofParameter<void> voidParameter;

	ofFbo fbo;


};

