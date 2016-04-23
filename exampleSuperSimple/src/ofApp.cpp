#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetVerticalSync(true);

	// we add this listener before setting up so the initial circle resolution is correct
	circleResolution.addListener(this, &ofApp::circleResolutionChanged);
	ringButton.addListener(this,&ofApp::ringButtonPressed);

	filled.set("bFill", true);
	radius.set( "radius", 140, 10, 300 );
	center.set("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight()));
	color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255));
	circleResolution.set("circleRes", 5, 3, 90);
	twoCircles.set("twoCircles", false);
	ringButton.set("ring");
	screenSize.set("screenSize", ofToString(ofGetWindowWidth()) + "x" + ofToString(ofGetWindowHeight()));

	gui.add(filled, radius, center, color, circleResolution, twoCircles, ringButton, screenSize);

	bHide = false;

	ring.load("ring.wav");
}

//--------------------------------------------------------------
void ofApp::exit(){
	ringButton.removeListener(this,&ofApp::ringButtonPressed);
}

//--------------------------------------------------------------
void ofApp::circleResolutionChanged(int & circleResolution){
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void ofApp::ringButtonPressed(){
	ring.play();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);

	if( filled ){
		ofFill();
	}else{
		ofNoFill();
	}

	ofSetColor(color);
	if(twoCircles){
		ofDrawCircle(center->x-radius*.5, center->y, radius );
		ofDrawCircle(center->x+radius*.5, center->y, radius );
	}else{
		ofDrawCircle((ofVec2f)center, radius );
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
