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

		ofxGuiContainer *containerExample;
		ofxGuiGroup *groupExample;
		ofxGuiPanel *panelExample;
		ofParameter<string> containerLabel, groupLabel, panelLabel;

		ofxGuiPanel *panel1;
		ofxGuiGroup *labels, *buttons, *specialSliders;

		//plotter
		ofParameter<ofPoint> sinus;
		ofParameter<float> randomVal;

		//textfields
		ofParameter<float> floatfieldVal;
		ofParameter<string> textfieldVal;

		ofParameter <bool> showHeaders;

		//sliders
		ofxGuiContainer* sliders;
		ofParameter <float> slider1Val, slider2Val, slider3Val, slider4Val;
		ofParameter <float> circularSliderVal;

		//rangeslider
		ofParameter<float> rangesliderStart, rangesliderEnd;

		//exponential slider
		ofParameter<float> exponentialSlider;

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

