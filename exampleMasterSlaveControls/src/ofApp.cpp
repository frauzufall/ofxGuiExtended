#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(120);

	/*
	 * create panels
	 */

	ofxPanel* panel1 = gui.addPanel("control");
	ofxPanel* panel2 = gui.addPanel();
	panel2->setPosition(250, 10);


	/*
	 * add masters
	 */

	ofxBaseGui* control = panel1->add(m_slider1.set("sinus",0,-1,1));
	control->setFillColor(ofColor::tomato);
	msc.addMaster(control);

	control = panel1->add(m_slider2.set("cosine",0,-1,1));
	control->setFillColor(ofColor::aqua);
	msc.addMaster(control);

	/*
	 * add slaves
	 */

	control = panel2->add(s_slider1.set("slave1",0,0,1));
	msc.addSlave(control);

	control = panel2->add(s_slider2.set("slave2",0,10,20));
	msc.addSlave(control);

	control = panel2->add(s_toggle.set("slave3",false));
	msc.addSlave(control);

	/*
	 * add regular slider
	 */

	panel2->add(s_slider3.set("regular slider",0,-1,1));

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
	m_slider1 = sin(ofGetElapsedTimef());
	m_slider2 = cos(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);

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
