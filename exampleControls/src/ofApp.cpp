#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetFrameRate(120);

    panel1.setup("extended gui");
    panel1.setBorderColor(ofColor::black);

    ofxLabelExtended::Config labelConfig;
    labelConfig.showName = false;
    panel1.add<ofxLabelExtended>(ofParameter<std::string>("LabelName", "extended Label"), labelConfig);

    panel1.add<ofxFpsPlotter>();

    /*
     * minimal button and toggle
     */
    ofxMinimalToggle::Config config;
    config.shape.height = 30;
    panel1.add<ofxMinimalToggle>(toggle_param.set("show header", true),config);
    toggle_param.addListener(this, &ofApp::toggleGroupHeader);
    panel1.add<ofxMinimalButton>(config);

    /*
     * rotary slider
     */
    rotary.setup("rotary");
    rotary.setBorderColor(ofColor::blanchedAlmond - ofColor(50));
    ofxRotarySlider<float>::Config rotaryConfig;
    rotaryConfig.shape.width = rotaryConfig.shape.height = 66;
    rotary.add<ofxFloatRotarySlider>(slider_param.set("slider", 0.5, 0, 1), rotaryConfig);
    rotary.getControl("slider")->setFillColor(ofColor::white);
    rotary.getControl("slider")->setBackgroundColor(ofColor::blanchedAlmond - ofColor(130));

    /*
     * matrix with only one allowed active toggle
     */
    matrix.setup("matrix",3);
    matrix.add<ofxMinimalToggle>(ofParameter<bool>("only",false));
    matrix.add<ofxMinimalToggle>(ofParameter<bool>("one",false));
    matrix.add<ofxMinimalToggle>(ofParameter<bool>("toggle",false));
    matrix.add<ofxMinimalToggle>(ofParameter<bool>("can",false));
    matrix.add<ofxMinimalToggle>(ofParameter<bool>("be",false));
    matrix.add<ofxMinimalToggle>(ofParameter<bool>("active",false));
    matrix.setBorderColor(ofColor::aquamarine);
    matrix.setElementHeight(26);
    matrix.allowMultipleActiveToggles(false);

    panel1.add(rotary);
    panel1.add(matrix);

    /*
     * horizontal panel with spacer
     */
    panel2.setup("horizontal", "", 260, 10);
    panel2.setLayout(ofxBaseGui::Horizontal);
    panel2.setBorderColor(ofColor::black);
    ofxToggle::Config toggleConfig;
    toggleConfig.shape.width = 100;
    toggleConfig.shape.height = 30;
    panel2.add(toggle1_param.set("toggle1", false), toggleConfig);
    panel2.add<ofxMinimalToggle>(toggle2_param.set("toggle2", false), toggleConfig);
    panel2.add<ofxGuiSpacer>(ofxGuiSpacer::Config(30));
    panel2.add<ofxMinimalToggle>(toggle3_param.set("toggle3", false), toggleConfig);
    panel2.add<ofxMinimalToggle>(toggle4_param.set("toggle4", false), toggleConfig);

    /*
     * panel with canvas
     */
    panel3.setup("canvas", "", 260, 90);
    img.load("images/ente.jpg");
    panel3.add<ofxGuiBaseDraws>(ofxGuiBaseDraws::Config("some texture", &img.getTexture()));

    panel4.setup("zoomable canvas", "", 500, 90);
    panel4.add<ofxGuiBaseDraws>(ofxGuiBaseDraws::Config("same texture", &img.getTexture()));

    panel5.setup("vertical sliders", "", 260, 280);
    panel5.setLayout(ofxBaseGui::Horizontal);
    panel5.add<ofxFloatVerticalSlider>(slider1_param.set("slider1", 1./7., 0, 1));
    panel5.add<ofxFloatVerticalSlider>(slider2_param.set("slider2", 5./7., 0, 1));
    panel5.add<ofxFloatVerticalSlider>(slider3_param.set("slider3", 4./7., 0, 1));
    panel5.add<ofxFloatVerticalSlider>(slider4_param.set("slider4", 6./7., 0, 1));
    panel5.getControl("slider1")->setSize(40, 130);
    panel5.getControl("slider2")->setSize(50, 130);
    panel5.getControl("slider3")->setSize(60, 130);
    panel5.getControl("slider4")->setSize(70, 130);

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
    //fps.update();
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
            /*if(matrix_params.at(0).get()) {
                ofSetColor(ofColor::burlyWood);
            }
            else {
                ofSetColor(ofColor::fromHex(0x2da1e3));
            }*/
        }
    }

    ofDrawRectangle(ofGetWindowRect());

    ofSetColor(255);

    panel1.draw();
    panel2.draw();
    panel3.draw();
    panel4.draw();
    panel5.draw();

}

void ofApp::toggleGroupHeader(bool &val) {
    rotary.setShowHeader(val);
    matrix.setShowHeader(val);
    panel1.setShowHeader(val);
    panel2.setShowHeader(val);
    panel3.setShowHeader(val);
    panel4.setShowHeader(val);
    panel5.setShowHeader(val);
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
