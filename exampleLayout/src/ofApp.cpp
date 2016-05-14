#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);

	gui.setupFlexBoxLayout();

	ofxGuiGroup* all = gui.addGroup("all", ofJson({
		{"position", "static"},
		{"height", "100%"},
		{"width", "100%"},
		{"show-header", false},
		{"background-color", "#000000"}
	}));

	// add header group
	ofxGuiGroup* header = all->addGroup("header", ofJson({
		{"flex-direction", "row"},
		{"show-header", false},
		{"height", 50},
		{"background-color", "#F40056"}
	}));
	header->addLabel("this is the header");

	// create tab container
	ofxGuiTabs* tabbed_pages = all->addTabs("tabbedpages", ofJson({{"flex", 1}}));

	// add a page to the tab container
	ofxGuiGroup* page1 = tabbed_pages->addGroup("page1", ofJson({
		{"background-color", "#2377BA"},
		{"flex-direction", "row"},
		{"padding", 10},
		{"flex-wrap", "wrap"}
	}));

	// add labels with random width to the first page
	for(int i = 0; i < 33; i++){
		page1->addLabel(ofToString(i), ofJson({
			{"border-width", 1},
			{"width", ofRandom(30, 130)}
		}));
	}

	// add two more pages without content
	tabbed_pages->addGroup("page2", ofJson({{"background-color", "#00CA98"}}));
	tabbed_pages->addGroup("page3", ofJson({{"background-color", "#ffaa00"}}));

//	add footer group
	ofxGuiGroup* footer = all->addGroup("footer", ofJson({
		{"flex-direction", "row"},
		{"show-header", false},
		{"height", 50},
		{"background-color", "#F47E00"}
	}));
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
