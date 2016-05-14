#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	testBool.set("bool", true);
	testFloat.set("float", 0.5, 0, 1);
	testColor.set("color", ofColor(120), ofColor(0,0), ofColor(255,255));
	testPoint.set("point", ofPoint(0.5,0.5), ofPoint(0,0), ofPoint(100,100));
	testString.set("string", "teststring");


	gui_flex.setupFlexBoxLayout();


	ofxGuiGroup* control = gui_control.addPanel("", ofJson({
		{"show-header", false},
		{"direction", "horizontal"}
	}));
	control->setExclusiveToggles(true);
	control->add(gui_box.getDocument()->getVisible().set("box layout", false));
	control->add(gui_flex.getDocument()->getVisible().set("flexbox layout", true));
	control->add<ofxGuiLabel>("(note: both cases should look identical)");


	vector<ofxGui*> guis;
	guis.push_back(&gui_box);
	guis.push_back(&gui_flex);

	for(ofxGui* gui : guis){

		vector<ofxGuiGroup*> panels;

		ofxGuiGroup* panel1 = gui->addPanel();
		panel1->setPosition(10, 80);
		panel1->loadTheme("theme_default.json");

		ofxGuiGroup* panel2 = gui->addPanel();
		panel2->loadTheme("theme_light.json");
		panel2->setPosition(panel1->getPosition().x + panel1->getWidth()+10, panel1->getPosition().y);

		panels.push_back(panel1);
		panels.push_back(panel2);

		for(ofxGuiGroup* panel : panels){
			panel->add(testBool);
			panel->add(testPoint);
			panel->add(testFloat);
			panel->add(testColor);
			panel->add(testString);
		}

		ofxGuiGroup* panel3 = gui->addPanel("", ofJson({
			{"show-header", false},
			{"direction", "horizontal"},
			{"flex-direction", "row"}
		}));
		panel3->setPosition(panel2->getPosition().x + panel2->getWidth()+10, panel2->getPosition().y);
		panel3->add(testBool);
		panel3->add(testBool);
		panel3->add(testBool);


		//give the flexbox layout a different color to be able to see that something happens when you change the layout
		if(gui == &gui_flex){
			ofJson config = {
				{
					"group-header", {
						 {"background-color", "#123456"}
					 }
				 }
			};
			panel1->setTheme(config);
			panel2->setTheme(config);

		}

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
