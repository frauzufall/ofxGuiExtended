#pragma once

#include "ofxGuiExtended.h"

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxGui gui;
		ofxGuiPanel *panel1, *panel2;

		ofParameterGroup parameters;
		ofParameter<float> size;
		ofParameter<int> number;
		ofParameter<ofVec2f> position;

		ofParameter<ofColor> color;
};
