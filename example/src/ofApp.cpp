#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofBackgroundGradient(ofColor::white, ofColor::gray);

	ofSetFrameRate(120);

	ofSetVerticalSync(true);

	position = ofPoint(ofRandomWidth(), ofRandomHeight());

	// initialize the parameters you want to work with, set minimal and maximal values
	moving.set("moving", true);
	speed.set("speed", 0.5, 0, 1);
	rotation.set("rotate", ofRandom(0,2*PI), 0, 2*PI);

	// add the parameters to the gui
	gui.add(moving, speed, rotation);

	// that's all you have to do to setup, update and show the panel
	// the rest of the example is just playing around with a fly

}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){

	// computing new position of the fly
	if(moving){
		position += direction;
	}
	direction = ofPoint(cos(rotation),sin(rotation))*speed.get();

	// making sure the fly stays inside of the window
	if(position.x < 0) position.x = 0;
	if(position.x > ofGetWindowWidth()) position.x = ofGetWindowWidth();
	if(position.y < 0) position.y = 0;
	if(position.y > ofGetWindowHeight()) position.y = ofGetWindowHeight();

}

//--------------------------------------------------------------

void ofApp::draw(){

	ofVec2f wingsize(60,30);

	// computing the wing animation
	float waving = 0;
	if(moving && speed > 0){
		waving = sin(ofGetElapsedTimef()*speed*50)*40;
	}
	ofPoint wingpos(waving, -wingsize.x);
	ofPoint wingpos_norm = wingpos.getNormalized();
	ofPoint p1 = wingpos_norm*wingsize.x+wingpos_norm.getRotated(90, ofVec3f(0,0,1))*wingsize.y/2;
	ofPoint p2 = wingpos_norm*wingsize.x-wingpos_norm.getRotated(90, ofVec3f(0,0,1))*wingsize.y/2;

	if(speed > 0){
		// draw direction arrow
		ofPoint arrowStart = position + direction.getNormalized()*15;
		ofPoint arrowEnd = arrowStart + direction*100;
		ofSetColor(0,100,255);
		ofDrawArrow(arrowStart, arrowEnd, 3);
	}

	ofPushMatrix();
		ofTranslate(position);
		ofRotateZ(ofRadToDeg(rotation));

		// draw body
		ofSetColor(0);
		ofFill();
		ofDrawCircle(ofPoint(0,0), 10);

		// draw wings
		ofNoFill();
		ofSetLineWidth(1);
		ofBeginShape();
		ofCurveVertex(0,0);
		ofCurveVertex(0,0);
		ofCurveVertex(p1.x, p1.y);
		ofCurveVertex(p2.x, p2.y);
		ofCurveVertex(0,0);
		ofCurveVertex(p1.x, -p1.y);
		ofCurveVertex(p2.x, -p2.y);
		ofCurveVertex(0,0);
		ofCurveVertex(0,0);
		ofEndShape();
	ofPopMatrix();

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
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
