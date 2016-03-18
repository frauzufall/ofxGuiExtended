#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	img.load("images/ente.jpg");


	panel1 = gui.addPanel("canvas");
	panel1->setPosition(260, 90);


	panel1->add<ofxGuiGraphics>("some texture", &img.getTexture());


	panel2 = gui.addPanel("zoomable canvas");
	panel2->setPosition(500, 90);
	panel2->add<ofxGuiZoomableGraphics>("same texture", &img.getTexture());

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw(){

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
