#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);


	ofxGuiGroup* all = gui.addGroup("all", ofJson({
		{"position", "static"},
		{"height", "600"},
		{"width", "600"},
		{"show-header", false},
		{"background-color", "#000000"}
	}));

	// add header group
	panel = all->addPanel("header", ofJson({
		{"flex-direction", "row"},
		{"show-header", true},
		{"height", 50},
		{"background-color", "#F40056"}
	}));
	panel->addLabel("this is the panel");

	// create tab container
	ofxGuiTabs* tabbed_pages = panel->addTabs("tabbedpages", ofJson());

	// add a page to the tab container
	ofxGuiGroup* page1 = tabbed_pages->addGroup("page1", ofJson({
		{"background-color", "#2377BA"},
		{"padding", 10},
	}));

	// add labels with random width to the first page
	for(int i = 0; i < 33; i++){
		page1->addLabel(ofToString(i), ofJson({
			{"border-width", 1},
			{"width", ofRandom(30, 130)}
		}));
	}

	// add two more pages

	param1.set("param1", 0.5, 0, 1);
	param2.set("param2", 0.5, 0, 1);


	ofxGuiGroup* page2  = tabbed_pages->addGroup("page2", ofJson({{"background-color", "#00CA98"}}));

	page2->add(param1);
	page2->add(param2);

	param3.set("param3", 0.5, 0, 1);
	param4.set("param4", 0.5, 0, 1);

	ofxGuiGroup* page3 = tabbed_pages->addGroup("page3", ofJson({{"background-color", "#ffaa00"}}));

	page3->add(param3);
	page3->add(param4);

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
	case 'r':
	panel->loadFromFile("settings.xml");
	break;
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
