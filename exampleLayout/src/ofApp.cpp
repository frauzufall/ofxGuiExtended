#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofxGuiGroup* all = gui.addGroup("", ofJson({{"position", "static"},
												{"height", "100%"},
												{"show-header", false},
												{"background-color", "#000000"}}));

	ofxGuiGroup* header = all->addGroup("header", ofJson({{"flex-direction", "row"},
														  {"show-header", false},
														  {"height", 50},
														  {"background-color", "#F40056"}}));
	header->addLabel("this is the header");

	// create tab container
	ofxGuiTabs* tabbed_pages = all->addTabs("tabbedpages", ofJson({{"flex", 1}}));

	// create pages and add them to tab layout
	tabbed_pages->addGroup("page1", ofJson({{"background-color", "#2377BA"}}));
	tabbed_pages->addGroup("page2", ofJson({{"background-color", "#00CA98"}}));
	tabbed_pages->addGroup("page3", ofJson({{"background-color", "#ffaa00"}}));

	ofxGuiGroup* footer = all->addGroup("footer", ofJson({{"flex-direction", "row"},
														  {"show-header", false},
														  {"height", 50},
														  {"background-color", "#F47E00"}}));
	footer->addLabel("this is the footer");



}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor::white, ofColor::gray);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
	 case 'f': {
		 ofToggleFullscreen();
		 break;
	 }

	 default:
		 break;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
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
