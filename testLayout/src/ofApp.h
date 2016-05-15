#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void drawMousePosition(ofEventArgs&);

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

		ofxGui gui_control, gui_box, gui_flex;

		ofParameter<bool> testBool;
		ofParameter<float> testFloat;
		ofParameter<std::string> testString;
		ofParameter<ofColor> testColor;
		ofParameter<ofPoint> testPoint;

		ofPoint dragStart;
		bool dragging;
		ofBitmapFont font;

};
