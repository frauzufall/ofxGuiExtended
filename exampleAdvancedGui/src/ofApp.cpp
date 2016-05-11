#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	activeName.set("element name", "");
	activeIndex.set("element index", -1);


	/*
	 *  panel without header and a button that toggles the visibility of all the other headers
	 */
	panel1 = gui.addPanel();
	panel1->setPosition(20,20);
	panel1->setShowHeader(false);

	/*
	 * toggle to show or hide header
	 */

	showHeaders.set("show/hide header", true);
	panel1->add(showHeaders, ofJson({{"type", "fullsize"}, {"text-align", "center"}}));

	panel1->addSpacer(0, 20);

	/*
	 * Plotter
	 */
	panel1->addFpsPlotter();
	auto sinusfunction = [&] (float x) { return sin(x); };

	sinus.set("sinus", ofPoint(0,0), ofPoint(0, -1), ofPoint(4*PI, 1));
	panel1->add<ofxGuiFunctionPlotter>(sinus, sinusfunction);

	panel1->add<ofxGuiValuePlotter>(randomVal.set("random value", 0, 0, 9), ofJson({{"precision", 2}}));

	panel1->addSpacer(0, 20);

	/*
	 * buttons
	 */
	buttons = panel1->addGroup("buttons");
	buttons->add<ofxGuiButton>("fullsize button", ofJson({{"type", "fullsize"}, {"text-align", "center"}}));
	buttons->add<ofxGuiButton>("checkbox button", ofJson({{"type", "checkbox"}}));
	buttons->add<ofxGuiButton>("radio button", ofJson({{"type", "radio"}}));

	panel1->addSpacer(0, 20);

	/*
	 *  input fields
	 */
	panel1->add<ofxGuiFloatInputField>(floatfieldVal.set("float input",3.5,0,500));
	panel1->add<ofxGuiTextField>(textfieldVal.set("text input","type in here"));

	/*
	 * ofParameterGroup example with radio toggles, listener to show current index and name
	 */

	colorParameters.setName("ofParameterGroup");
	colorParameters.add(color0.set("mediumSlateBlue",false));
	colorParameters.add(color1.set("tomato",false));
	colorParameters.add(color2.set("mediumAquaMarine",false));
	colorParameters.add(color3.set("steelBlue",false));

	colorPanel = gui.addPanel("header color", ofJson({{"width", 270}}));
	colorPanel->setPosition(panel1->getShape().getTopRight()+ofPoint(20,0));
	colorToggles = colorPanel->addGroup(colorParameters);
	colorToggles->setExclusiveToggles(true);
	colorToggles->setConfig(ofJson({{"type", "radio"}}));

	/*
	 *  labels
	 */
	labels = colorPanel->addGroup("labels");
	labels->add(activeName);
	labels->add<ofxGuiIntLabel>(activeIndex);
	labels->add<ofxGuiLabel>("text without parameter");


	/*
	 *  sliders
	 */
	sliders = gui.addPanel("vertical sliders", ofJson({{"direction", "horizontal"}, {"show-header", false}}));
	sliders->setPosition(colorPanel->getShape().getTopRight()+ofPoint(20,0));

	sliders->add(slider1Val.set("slider1", 1. / 7., 0, 1), ofJson({{"width", 40}, {"height", 130}}));
	sliders->add(slider2Val.set("slider2", 5. / 7., 0, 1), ofJson({{"width", 50}, {"height", 130}}));
	sliders->add(slider3Val.set("slider3", 4. / 7., 0, 1), ofJson({{"width", 60}, {"height", 130}}));
	sliders->add(slider4Val.set("slider4", 6. / 7., 0, 1), ofJson({{"width", 70}, {"height", 130}}));

	sliders->add(circularSliderVal.set("slider", 0.5, 0, 1), ofJson({{"type", "circular"}, {"width", 130}, {"height", 130}, {"precision", 2}}));


	/*
	 * adding listeners
	 */

	showHeaders.addListener(this, &ofApp::toggleGroupHeader);
	colorToggles->getActiveToggleIndex().addListener(this, &ofApp::setHeaderColors);
	colorToggles->setActiveToggle(3);

}

//--------------------------------------------------------------
void ofApp::exit(){
	showHeaders.removeListener(this, &ofApp::toggleGroupHeader);
	colorToggles->getActiveToggleIndex().removeListener(this, &ofApp::setHeaderColors);
}

//--------------------------------------------------------------
void ofApp::update(){
	randomVal.set(ofRandom(randomVal.getMin(), randomVal.getMax()));
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor::white, ofColor::gray);

}

void ofApp::setHeaderColors(int& index){

	activeIndex = index;
	activeName = colorParameters.get(index).getName();

	ofColor c;
	switch(index){
		case 0: c = ofColor::mediumSlateBlue; break;
		case 1: c = ofColor::tomato; break;
		case 2: c = ofColor::mediumAquaMarine; break;
		default: case 3: c = ofColor::steelBlue; break;

	}

	labels->getHeader()->setBackgroundColor(c);
	buttons->getHeader()->setBackgroundColor(c);
	sliders->getHeader()->setBackgroundColor(c);
	colorPanel->getHeader()->setBackgroundColor(c);
	colorToggles->getHeader()->setBackgroundColor(c);

	color = c;
}

void ofApp::toggleGroupHeader(bool & val){
	labels->setShowHeader(val);
	buttons->setShowHeader(val);
	colorPanel->setShowHeader(val);
	colorToggles->setShowHeader(val);
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
