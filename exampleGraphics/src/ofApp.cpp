#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	// load image
	img.load("images/ente.jpg");
	fbo.allocate(img.getWidth(), img.getHeight(), GL_RGB);

	// add panel 1
	ofxGuiGroup* panel1 = gui.addPanel("graphics");
	panel1->setPosition(260, 90);

	// add image texture with fixed height
	panel1->add<ofxGuiGraphics>("some texture", &img.getTexture(), ofJson({{"height", 200}}));

	// add panel 2
	ofxGuiGroup* panel2 = gui.addPanel("zoomable graphics");
	panel2->setPosition(500, 90);

	// add image texture and get the pointer to this graphics gui element
	ofxGuiGraphics* graphics = panel2->add<ofxGuiZoomableGraphics>("same texture", &img.getTexture());
	// set image heigth according to width and ratio if the image
	graphics->setAutoHeight();
	// add fbo texture
	graphics = panel2->add<ofxGuiZoomableGraphics>("fbo", &fbo.getTexture());
	graphics->setAutoHeight();

}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {
	fbo.begin();
		ofSetColor(ofColor::fromHsb(0, sin(ofGetElapsedTimef())*255, 255));
		img.draw(0,0);
	fbo.end();
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
