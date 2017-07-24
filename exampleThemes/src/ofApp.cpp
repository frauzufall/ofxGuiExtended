#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	renderer1.setup("renderer1");
	renderer2.setup("renderer2");
	renderer3.setup("renderer3");

	gui.setupFlexBoxLayout();

	ofxGuiPanel* panel1 = gui.addPanel(renderer1.parameters);
	panel1->loadTheme("theme_default.json", true);

	ofxGuiPanel* panel2 = gui.addPanel(renderer2.parameters);
	panel2->loadTheme("theme_light.json", true);
	panel2->setPosition(panel1->getShape().getTopRight()+ofPoint(10, 0));

	ofxGuiPanel* panel3 = gui.addPanel(renderer3.parameters);
	panel3->loadTheme("theme3.json", true);
	panel3->setPosition(panel2->getShape().getTopRight()+ofPoint(10, 0));

}


//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	renderer1.draw();
	renderer2.draw();
	renderer3.draw();
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
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
