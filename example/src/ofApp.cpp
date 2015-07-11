#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    panel.setup("extended gui");

    minimal.setup("minimal");
    minimal.setBorderColor(ofColor::red);
    minimal.add(new ofxMinimalToggle(toggle_param.set("toggle", false), 0, 30));
    minimal.add(new ofxMinimalToggle(toggle_param.set("toggle", false), 0, 30));
    minimal.add(new ofxMinimalButton("button", 0, 30));

    horizontal.setup("horizontal");
    horizontal.setAlignHorizontal();
//    horizontal.showHeader(true);
    horizontal.setBorderColor(ofColor::aquamarine);
    horizontal.add(new ofxMinimalToggle(toggle1_param.set("toggle1", false), 80, 30));
    horizontal.add(new ofxMinimalToggle(toggle2_param.set("toggle2", false), 80, 30));
    horizontal.add(new ofxMinimalToggle(toggle3_param.set("toggle3", false), 80, 30));

    rotary.setup("rotary");
    rotary.setBorderColor(ofColor::blanchedAlmond);
    rotary.add(new ofxRotarySlider<float>(slider_param.set("slider", 0.5, 0, 1), 66,66));
    rotary.getControl("slider")->setFillColor(ofColor::white);
    rotary.getControl("slider")->setBackgroundColor(ofColor::blanchedAlmond - ofColor(130));

    panel.add(&minimal);
    panel.add(&rotary);
    panel.add(&horizontal);

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
