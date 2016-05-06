#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:

		ofxGui gui;

		ofxGuiPanel *panel1;
		ofxGuiGroup *labels, *buttons;

		//plotter
		ofParameter<ofPoint> sinus;
		ofParameter<float> randomVal;

		//textfields
		ofParameter<float> floatfieldVal;
		ofParameter<string> textfieldVal;

		ofParameter <bool> showHeaders;

		//sliders
		ofxGuiPanel* sliders;
		ofParameter <float> slider1Val, slider2Val, slider3Val, slider4Val;
		ofParameter <float> circularSliderVal;

		//color panel
		ofxGuiPanel* colorPanel;
		ofxGuiGroup *colorToggles;

		ofParameterGroup colorParameters;
		ofParameter<bool> color0;
		ofParameter<bool> color1;
		ofParameter<bool> color2;
		ofParameter<bool> color3;

		ofParameter<string> activeName;
		ofParameter<int> activeIndex;

		ofColor color;

		void toggleGroupHeader(bool & val);
		void setHeaderColors(int& index);

};

