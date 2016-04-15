#pragma once

#include "ofxBaseGui.h"
#include "ofImage.h"

class ofxFunctionPlotter : public ofxBaseGui {
	public:

		ofxFunctionPlotter(ofParameter<ofPoint> value, const ofJson & config = ofJson());
		ofxFunctionPlotter(ofParameter<ofPoint> value, std::function<float (float)> function, const ofJson & config = ofJson());

		virtual ~ofxFunctionPlotter();

		void setup();

		void setPlotterStrokeWidth(float width);
		void setFunction(std::function<float (float)> function);

		virtual void setFillColor(const ofColor& color) override;
		virtual void setFillColor(const ofColor& minColor, const ofColor& maxColor);

		void setDecimalPlace(int place);

		virtual ofAbstractParameter & getParameter() override;

	protected:

		virtual void _setConfig(const ofJson & config) override;
		virtual void render() override;
		virtual void generateDraw() override;

		ofVboMesh textMesh;
		vector <float> buffer;
		ofPath plot;
		ofParameter<float> plotterStrokeWidth;
		ofParameter <string> label;
		ofParameter<ofPoint> value;
		std::function<float (float)> function;

		ofParameter<int> decimalPlace;
		ofColor minColor, maxColor;
		ofImage background_gradient;

		void valueChanged(ofPoint & value);

};
