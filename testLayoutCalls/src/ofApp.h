#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGuiExtended.h"

struct GuiObj{
	int id;
	ofRectangle shape;
	std::string name;
	vector<ofPtr<GuiObj>> children;
	bool hasParent = false;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawObj(ofxGuiContainer* container, ofPtr<GuiObj> obj, float width, ofPoint pos);
		void exit();

		int totalLeafSize(ofPtr<GuiObj> obj);

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

		vector<ofPtr<GuiObj>> objs;

		vector<ofPtr<GuiObj>> objsSorted;

		ofxGui gui, gui2;
		ofxGuiContainer *labels;
		ofxGuiElement *activeElement;

		ofParameter<float> process, speed;

		float lastUpdate;

};
