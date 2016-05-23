#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	parameters.setName("random values");
	parameters.add(size.set("size",10,0,100));
	parameters.add(number.set("number",2,1,20));
	parameters.add(position.set("position",ofPoint(42,42), ofPoint(0,0), ofPoint(100,100)));
	parameters.add(color.set("color",ofColor(127),ofColor(0,0),ofColor(255)));

	panel1 = gui.addPanel(parameters);
	panel1->loadTheme("theme_default.json");

	panel2 = gui.addPanel(parameters);
	panel2->loadTheme("theme_light.json");
	panel2->setPosition(ofGetWidth()-panel2->getWidth()-20, 20);

}


//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
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
	panel2->setPosition(w-panel2->getWidth()-20, 20);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
