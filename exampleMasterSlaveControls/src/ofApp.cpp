#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    panel1.setup("control");
    panel2.setup("slave", "", 250, 10);


    panel1.ofxGuiGroup::add(m_slider1.set("sinus",0,-1,1));
    panel1.getControl("sinus")->setFillColor(ofColor::tomato);
    ofxMasterSlaveControl::get().addMaster(panel1.getControl("sinus"));

    panel1.ofxGuiGroup::add(m_slider2.set("cosine",0,-1,1));
    panel1.getControl("cosine")->setFillColor(ofColor::aqua);
    ofxMasterSlaveControl::get().addMaster(panel1.getControl("cosine"));

    panel2.ofxGuiGroup::add(s_slider1.set("slave1",0,-1,1));
    ofxMasterSlaveControl::get().addSlave(panel2.getControl("slave1"));

    panel2.ofxGuiGroup::add(s_slider2.set("slave2",0,-1,1));
    ofxMasterSlaveControl::get().addSlave(panel2.getControl("slave2"));

    panel2.ofxGuiGroup::add(s_slider3.set("regular slider",0,-1,1));

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

    panel1.draw();
    panel2.draw();

    ofxMasterSlaveControl::get().draw();

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
