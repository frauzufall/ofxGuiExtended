#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    page1.setup("page 1", "", 0, 0);
    page1.setSize(ofGetWindowWidth(), ofGetWindowHeight());
    page1.setBorderColor(0);

    /*
     * minimal button and toggle
     */
    panel1.setup("panel1");
    panel1.add(new ofxMinimalToggle(toggle_param.set("toggle", true), 0, 30));
    panel1.add(new ofxMinimalButton("button", 0, 30));

    rotary.setup("rotary");
    rotary.add(new ofxFloatRotarySlider(slider_param.set("slider", 0.5, 0, 1), 66,66));

    matrix_params.push_back(ofParameter<bool>("all",false));
    matrix_params.push_back(ofParameter<bool>("toggles",false));
    matrix_params.push_back(ofParameter<bool>("can",false));
    matrix_params.push_back(ofParameter<bool>("be",false));
    matrix_params.push_back(ofParameter<bool>("active",false));
    matrix.setup("matrix",2);
    matrix.allowMultipleActiveToggles(true);
    for(uint i = 0; i < matrix_params.size(); i++) {
        matrix.add(new ofxMinimalToggle(matrix_params.at(i)));
    }

    panel1.add(&matrix);
    panel1.add(&rotary);

    /*
     * horizontal panel with spacer
     */
    panel2.setup("horizontal", "", 260, 10);
    panel2.setAlignHorizontal();
    panel2.add(new ofxMinimalToggle(toggle2_param.set("toggle2", false), 0, 30));
    panel2.add(new ofxMinimalToggle(toggle3_param.set("toggle3", false), 0, 30));
    panel2.add(new ofxMinimalToggle(toggle4_param.set("toggle4", false), 0, 30));

    page1.add(&panel1);
    page1.add(&panel2);

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);

    page1.draw();

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
