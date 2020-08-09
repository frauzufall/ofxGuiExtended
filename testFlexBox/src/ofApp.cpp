#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	gui.setupFlexBoxLayout();

	ofxGuiGroup* all = gui.addGroup("", ofJson({
		{"flex-direction", "column"},
		{"background-color", "#rgba(0,0,0,0)"},
		{"flex", 1},
		{"margin", 0},
		{"padding", 0},
		{"width", "100%"},
		{"show-header", false},
		{"position", "static"}
	}));

	ofxGuiGroup* control = all->addGroup("control", ofJson({
		{"show-header", false},
		{"flex-direction", "row"},
		{"height", 50}
	}));

	ofJson controlGroupConfig = {{"width", 200}};

	justifyContent = control->addGroup("justify-content", controlGroupConfig);
	justifyContent->add<ofxGuiToggle>("flex-start");
	justifyContent->add<ofxGuiToggle>("flex-end");
	justifyContent->add<ofxGuiToggle>("center");
	justifyContent->add<ofxGuiToggle>("space-between");
	justifyContent->add<ofxGuiToggle>("space-around");
	justifyContent->setExclusiveToggles(true);
	justifyContent->setActiveToggle(0);


	alignItems = control->addGroup("align-items", controlGroupConfig);
	alignItems->add<ofxGuiToggle>("stretch");
	alignItems->add<ofxGuiToggle>("flex-start");
	alignItems->add<ofxGuiToggle>("flex-end");
	alignItems->add<ofxGuiToggle>("center");
	alignItems->setExclusiveToggles(true);
	alignItems->setActiveToggle(0);

	alignContent = control->addGroup("align-content", controlGroupConfig);
	alignContent->add<ofxGuiToggle>("stretch");
	alignContent->add<ofxGuiToggle>("flex-start");
	alignContent->add<ofxGuiToggle>("flex-end");
	alignContent->add<ofxGuiToggle>("center");
	alignContent->add<ofxGuiToggle>("space-between");
	alignContent->add<ofxGuiToggle>("space-around");
	alignContent->setExclusiveToggles(true);
	alignContent->setActiveToggle(0);

	wrapItems = control->addGroup("flex-wrap", controlGroupConfig);
	wrapItems->add<ofxGuiToggle>("wrap");
	wrapItems->add<ofxGuiToggle>("nowrap");
	wrapItems->setExclusiveToggles(true);
	wrapItems->setActiveToggle(0);

	ofxGuiGroup* groupsGroup = all->addGroup("", ofJson({
		{"flex-direction", "row"},
		{"flex", 1},
		{"show-header", false}
	}));

	groups.push_back(groupsGroup->addGroup("flex-direction: column", ofJson({
		{"flex-direction", "column"},
		{"flex", 1},
		{"flex-wrap", "wrap"},
		{"show-header", false},
		{"background-color", "#F40056"},
	})));

	groups.push_back(groupsGroup->addGroup("flex-direction: row", ofJson({
		{"flex-direction", "row"},
		{"flex", 1},
		{"flex-wrap", "wrap"},
		{"show-header", false},
		{"background-color", "#2377BA"}
	})));

	for(ofxGuiGroup* group : groups){
		for(int i = 0; i < 22; i++){
			group->addLabel(ofToString(i), ofJson({
				{"border-width", 1},
				{"width", ofToString(ofRandom(3, 20)) + ofToString("%")},
				{"height", ofToString(ofRandom(3, 20)) + ofToString("%")}
			}));
		}
	}

	justifyContent->getActiveToggleIndex().addListener(this, &ofApp::setJustifyContent);
	alignItems->getActiveToggleIndex().addListener(this, &ofApp::setAlignItems);
	alignContent->getActiveToggleIndex().addListener(this, &ofApp::setAlignContent);
	wrapItems->getActiveToggleIndex().addListener(this, &ofApp::setWrapItems);

}

void ofApp::exit(){
	justifyContent->getActiveToggleIndex().removeListener(this, &ofApp::setJustifyContent);
	alignItems->getActiveToggleIndex().removeListener(this, &ofApp::setAlignItems);
	alignContent->getActiveToggleIndex().removeListener(this, &ofApp::setAlignContent);
	wrapItems->getActiveToggleIndex().removeListener(this, &ofApp::setWrapItems);
}

void ofApp::setAlignContent(int &index){
	std::string value;
	switch(index){
		case 1:	value = "flex-start"; break;
		case 2:	value = "flex-end"; break;
		case 3:	value = "center"; break;
		case 4:	value = "space-between"; break;
		case 5:	value = "space-around"; break;
		default: case 0: value = "stretch"; break;
	}

	for(ofxGuiGroup* group : groups){
		group->setConfig(ofJson({{"align-content", value}}));
	}
}

void ofApp::setAlignItems(int &index){
	std::string value;
	switch(index){
		case 1:	value = "flex-start"; break;
		case 2:	value = "flex-end"; break;
		case 3:	value = "center"; break;
		default: case 0: value = "stretch"; break;
	}

	for(ofxGuiGroup* group : groups){
		group->setConfig(ofJson({{"align-items", value}}));
	}
}

void ofApp::setJustifyContent(int &index){
	std::string value;
	switch(index){
		case 1:	value = "flex-end"; break;
		case 2:	value = "center"; break;
		case 3:	value = "space-between"; break;
		case 4:	value = "space-around"; break;
		default: case 0: value = "flex-start"; break;
	}

	for(ofxGuiGroup* group : groups){
		group->setConfig(ofJson({{"justify-content", value}}));
	}
}

void ofApp::setWrapItems(int &index){
	std::string value;
	switch(index){
		case 1:	value = "nowrap"; break;
		default: case 0:	value = "wrap"; break;
	}

	for(ofxGuiGroup* group : groups){
		group->setConfig(ofJson({{"flex-wrap", value}}));
	}
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
