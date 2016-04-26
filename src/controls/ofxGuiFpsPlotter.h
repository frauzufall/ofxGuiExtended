#pragma once

#include "ofxGuiValuePlotter.h"

class ofxGuiFpsPlotter : public ofxGuiValuePlotter {
	public:
		ofxGuiFpsPlotter();
		ofxGuiFpsPlotter(const ofJson & config);
		ofxGuiFpsPlotter(std::string label, float minValue, float maxValue, int plotSize = 100, float width = defaultWidth, float height = defaultHeight);

		virtual ~ofxGuiFpsPlotter();

		void setup();
		void update(ofEventArgs &);

	protected:

};
