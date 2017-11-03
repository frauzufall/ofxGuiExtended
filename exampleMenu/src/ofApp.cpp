#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	data.add(addCircle.set("draw circle"));
	data.add(addRectangle.set("draw rectangle"));

	circleSettings.setName("circle");
	circleSettings.add(circleRadius.set("radius", 20, 1, 100));
	circleSettings.add(circleColor.set("color", ofColor(0,255,255,100), ofColor(0,0,0,0), ofColor(255,255,255,255)));

	rectangleSettings.setName("rectangle");
	rectangleSettings.add(rectangleWidth.set("width", 20, 1, 100));
	rectangleSettings.add(rectangleHeight.set("height", 20, 1, 100));
	rectangleSettings.add(rectangleColor.set("color", ofColor(255,0,255,100), ofColor(0,0,0,0), ofColor(255,255,255,255)));

	settings.setName("settings");
	settings.add(circleSettings);
	settings.add(rectangleSettings);
	data.add(settings);

	unusedParameters.setName("unused");
	unusedParameters.add(labelParameter.set("std::string"));
	unusedParameters.add(voidParameter.set("void"));
	unusedParameters.add(boolParameter.set("bool", false));
	unusedParameters.add(floatParameter.set("float", 0.5,0,1));
	unusedParameters.add(pointParameter.set("ofPoint", ofPoint(0.5,0.5,0.5),ofPoint(0,0,0), ofPoint(1,1,1)));
	unusedParameters.add(colorParameter.set("ofColor", ofColor(255,255,0,255),ofColor(0,0,0,0), ofColor(255,255,255,255)));
	unusedParameters.add(rectParameter.set("ofRectangle",
										   ofRectangle(100,100,100,100),
										   ofRectangle(0,0,0,0),
										   ofRectangle(ofGetWidth(), ofGetHeight(), ofGetWidth(), ofGetHeight())));

	data.add(unusedParameters);

	ofxGuiContainer* menu = gui.addMenu(data);

	menu->loadTheme("theme_light.json", true);

	addCircle.addListener(this, &ofApp::drawCircle);
	addRectangle.addListener(this, &ofApp::drawRectangle);

	fbo.begin();
	ofSetColor(200);
	ofFill();
	ofDrawRectangle(0,0,fbo.getWidth(), fbo.getHeight());
	fbo.end();

}

void ofApp::drawCircle(){
	fbo.begin();

		ofSetLineWidth(10);
		ofNoFill();
		ofSetColor(circleColor);
		ofDrawCircle(ofRandom(fbo.getWidth()), ofRandom(fbo.getHeight()), circleRadius);

	fbo.end();
}

void ofApp::drawRectangle(){
	fbo.begin();

		ofSetLineWidth(10);
		ofNoFill();
		ofSetColor(rectangleColor);
		ofDrawRectangle(ofRandom(fbo.getWidth()), ofRandom(fbo.getHeight()), rectangleWidth, rectangleHeight);

	fbo.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key) {
	case 'f': {
		ofToggleFullscreen();
		break;
	}
	default: break;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs &args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
