#pragma once

#include "ofxGuiValuePlotter.h"

class ofxGuiFpsPlotter : public ofxGuiValuePlotter {
	public:
		ofxGuiFpsPlotter();
		ofxGuiFpsPlotter(const ofJson & config);
		ofxGuiFpsPlotter(std::string label, float minValue, float maxValue, int plotSize = 100, const ofJson & config = ofJson());

		virtual ~ofxGuiFpsPlotter();

		void update(ofEventArgs &);

	protected:

		void setup();

};
