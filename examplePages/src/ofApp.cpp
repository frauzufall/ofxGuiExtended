#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    setupPage1();
    setupPage2();
    setupPage3();

    page.setup("single page");
    page.setSize(300, 300);
    page.add(&panel3);
//    page.add(&fps);

    page1.setup("page 1");
    page1.add(&panel1);
    page1.add(&rotary);

    page2.setup("page 2");
    page2.add(&panel2);

    tabbed_pages.setup("tabbed pages", "", page.getShape().getRight()+10);
    tabbed_pages.setSize(500, 300);
    tabbed_pages.add(&page1);
    tabbed_pages.add(&page2);

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

    page.draw();

    tabbed_pages.draw();

}

void ofApp::setupPage1() {
    panel1.setup("panel1");

    matrix.setup("matrix",2);
    for(unsigned int i = 0; i < 10; i++) {
        matrix.add((new ofxMinimalToggle())->setup("toggle"+ofToString(i), false));
    }
    panel1.add(new ofxMinimalButton("button", 0, 30));
    panel1.add(&matrix);

    rotary.setup("rotary", "", panel1.getShape().getRight()+20, 20);
    rotary.add(new ofxFloatRotarySlider(slider_param.set("slider", 0.5, 0, 1), 66,66));
}

void ofApp::setupPage2() {
    panel2.setup("horizontal", "", 260, 10);
    panel2.setAlignHorizontal();
    panel2.showHeader(false);
    ofParameter<bool> toggle2_param, toggle3_param, toggle4_param;
    panel2.add(new ofxMinimalToggle(toggle2_param.set("toggle2", false), 0, 30));
    panel2.add(new ofxMinimalToggle(toggle3_param.set("toggle3", false), 0, 30));
    panel2.add(new ofxMinimalToggle(toggle4_param.set("toggle4", false), 0, 30));
}

void ofApp::setupPage3() {
    panel3.setup("just some toggles", "", 60, 66);
    panel3.setAlignHorizontal();
    ofParameter<bool> toggle2_param, toggle3_param, toggle4_param;
    panel3.add(new ofxMinimalToggle(toggle2_param.set("toggle2", false), 0, 30));
    panel3.add(new ofxMinimalToggle(toggle3_param.set("toggle3", false), 0, 30));
    panel3.add(new ofxMinimalToggle(toggle4_param.set("toggle4", false), 0, 30));

//    fps.setup(100, 30);
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
