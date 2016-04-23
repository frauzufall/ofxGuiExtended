#include "ofApp.h"
#include "FlexBoxLayout.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);

//	ofxGuiGroup* tabs = gui.addGroup("tabbedpages");

//	tabs->setConfig(ofJson({
//							   {"flex-direction", "column"}
//						   }));


//	tabs->setSize(800,600);

//	ofxGuiGroup* group = tabs->addGroup("tabs", ofJson({
//													   {"align-self", "stretch"},
//													   {"height", 50},
//													   {"flex-direction", "row"},
//													   {"align-items", "flex-end"},
//													   {"justify-content", "flex-start"}
//												   }));



//	for(int i = 0; i < 3; i++){

//		ofxToggle* label = group->add<ofxToggle>(ofToString(i,2), ofJson({{"width", ofRandom(50,50)}, {"height", 20}, {"type", "fullsize"}}));
//		label->setBackgroundColor(ofColor(ofRandom(0,255), ofRandom(0,255),ofRandom(0,255)));
//	}

//	tabs->addGroup("page1", ofJson({
//								  {"flex", "auto"},
//								  {"align-self", "stretch"}
//							  }));



//	ofxGuiGroup* tabs2 = gui.addGroup("tabbedpages2");
//	tabs2->setPosition(tabs->getShape().getTopRight());
//	tabs2->setSize(800,600);

//	ofxGuiGroup* group2 = tabs2->addGroup("tabs2", ofJson({
//													   {"align-self", "stretch"},
//													   {"height", 50},
//													   {"flex-direction", "row"},
//													   {"align-items", "flex-end"},
//													   {"justify-content", "flex-start"}
//												   }));

//	tabs2->setConfig(ofJson({
//							   {"flex-direction", "column"}
//						   }));

//	for(int i = 0; i < 3; i++){

//		ofxToggle* label = group2->add<ofxToggle>(ofToString(i,2), ofJson({{"width", ofRandom(50,50)}, {"height", 20}, {"type", "fullsize"}}));
//		label->setBackgroundColor(ofColor(ofRandom(0,255), ofRandom(0,255),ofRandom(0,255)));
//	}

//	tabs2->addGroup("page2", ofJson({
//									  {"flex", "auto"},
//									  {"align-self", "stretch"}
//								  }));

	/*
	 *  create tab container
	 */
	tabbed_pages = gui.addTabs("tabbedpages");
	tabbed_pages->setSize(800,600);

	/*
	 *  create pages and add them to tab layout
	 */
	page1 = tabbed_pages->addPanel("page1", ofJson({{"show-header", false}}));
	page2 = tabbed_pages->addPanel("page2", ofJson({{"show-header", false}}));
	page3 = tabbed_pages->addPanel("page3", ofJson({{"show-header", false}}));

	/*
	 *  fill page 1
	 */
	vector<ofJson> choices;
	choices.push_back({{"float", "left"}, {"background-color", "#2da1e3"}});
	choices.push_back({{"float", "right"}, {"background-color", "#0ff"}});
	choices.push_back({{"float", "none"}, {"background-color", "rgba(255,0,0,0.7)"}});

	for(int i = 0; i < 10; i++){

		ofRandomize(choices);

		ofxLabel* label = page1->add<ofxLabel>(ofToString(i,2), choices[0]);
		label->setPercentalWidth(false);
		label->setSize((int)ofRandom(100,200), (int)ofRandom(60, 80));
		label->setMargin(1);
	}

	/*
	 *  raise the chance to create left or right floating elements
	 */
	choices.push_back({{"float", "left"}, {"background-color", "#2da1e3"}});
	choices.push_back({{"float", "right"}, {"background-color", "#0ff"}});

	/*
	 *  fill page 2
	 */
	for(int i = 0; i < 20; i++){

		ofRandomize(choices);

		ofxLabel* label = page2->add<ofxLabel>(ofToString(i,2), choices[0]);
		label->setPercentalWidth(false);
		label->setSize((int)ofRandom(70,50), (int)ofRandom(30, 50));
		label->setMargin(1);
	}

	/*
	 *  fill page 3
	 */
	for(int i = 0; i < 30; i++){

		ofRandomize(choices);

		ofxLabel* label = page3->add<ofxLabel>(ofToString(i,2), choices[0]);
		label->setPercentalWidth(false);
		label->setSize((int)ofRandom(130,150), (int)ofRandom(30, 50));
		label->setMargin(1);
	}


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
