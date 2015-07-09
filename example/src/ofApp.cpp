#include "ofApp.h"
#include "ofxGuiExtended.h"

//--------------------------------------------------------------
void ofApp::setup(){

    panel.setup("extended gui");
    minimals.setup("minimal");

    minimals.add(new ofxMinimalToggle(toggle_param.set("toggle", false), 0, 30));
    minimals.add(new ofxMinimalButton("button", 0, 30));

    panel.add(&minimals);

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw(){

    if(toggle_param.get()) {
        ofSetColor(ofColor::royalBlue);
        ofDrawRectangle(ofGetWindowRect());
    }
    else {
        ofSetColor(ofColor::black);
        ofDrawRectangle(ofGetWindowRect());
    }

    panel.draw();

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
