#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetFrameRate(120);

    panel1.setup("extended gui");
    panel1.setBorderColor(ofColor::black);

    panel1.add((new ofxLabelExtended())->setup("LabelName", "extended Label")->setShowLabelName(false));

    panel1.add(fps.setup(0,120));

    /*
     * minimal button and toggle
     */
    panel1.add(new ofxMinimalToggle(toggle_param.set("show header", true), 0, 30));
    toggle_param.addListener(this, &ofApp::toggleGroupHeader);
    panel1.add(new ofxMinimalButton("button", 0, 30));

    /*
     * rotary slider
     */
    rotary.setup("rotary");
    rotary.setBorderColor(ofColor::blanchedAlmond - ofColor(50));
    rotary.add(new ofxFloatRotarySlider(slider_param.set("slider", 0.5, 0, 1), 66,66));
    rotary.getControl("slider")->setFillColor(ofColor::white);
    rotary.getControl("slider")->setBackgroundColor(ofColor::blanchedAlmond - ofColor(130));

    /*
     * matrix with only one allowed active toggle
     */
    matrix_params.push_back(ofParameter<bool>("only",false));
    matrix_params.push_back(ofParameter<bool>("one",false));
    matrix_params.push_back(ofParameter<bool>("toggle",false));
    matrix_params.push_back(ofParameter<bool>("can",false));
    matrix_params.push_back(ofParameter<bool>("be",false));
    matrix_params.push_back(ofParameter<bool>("active",false));
    matrix.setup("matrix",3);
    matrix.clear();
    for(unsigned int i = 0; i < matrix_params.size(); i++) {
        matrix.add(new ofxMinimalToggle(matrix_params.at(i)));
    }
    matrix.setBorderColor(ofColor::aquamarine);
    matrix.setElementHeight(26);
    matrix.allowMultipleActiveToggles(false);

    panel1.add(&rotary);
    panel1.add(&matrix);

    /*
     * horizontal panel with spacer
     */
    panel2.setup("horizontal", "", 260, 10);
    panel2.setAlignHorizontal();
    panel2.setBorderColor(ofColor::black);
    panel2.add(new ofxToggle(toggle1_param.set("toggle1", false), 100, 30));
    panel2.add(new ofxMinimalToggle(toggle2_param.set("toggle2", false), 0, 30));
    panel2.add(new ofxMinimalToggle(toggle3_param.set("toggle3", false), 0, 30));
    panel2.add(new ofxGuiSpacer(30));
    panel2.add(new ofxMinimalToggle(toggle4_param.set("toggle4", false), 0, 30));

    /*
     * panel with canvas
     */
    panel3.setup("canvas", "", 260, 90);
    img.load("images/ente.jpg");
    canvas.setup("some texture", &img.getTexture());
    panel3.add(&canvas);

    panel4.setup("zoomable canvas", "", 500, 90);
    zcanvas.setup("same texture", &img.getTexture());
    panel4.add(&zcanvas);

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
    fps.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    if(toggle1_param.get()) {
        ofSetColor(ofColor::royalBlue);
    }
    else {
        if(toggle2_param.get()) {
            ofSetColor(ofColor::azure);
        }
        else {
            if(matrix_params.at(0).get()) {
                ofSetColor(ofColor::burlyWood);
            }
            else {
                ofSetColor(ofColor::fromHex(0x2da1e3));
            }
        }
    }

    ofDrawRectangle(ofGetWindowRect());

    ofSetColor(255);

    panel1.draw();
    panel2.draw();
    panel3.draw();
    panel4.draw();

}

void ofApp::toggleGroupHeader(bool &val) {
    rotary.setShowHeader(val);
    matrix.setShowHeader(val);
    panel1.setShowHeader(val);
    panel2.setShowHeader(val);
    panel3.setShowHeader(val);
    panel4.setShowHeader(val);
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
