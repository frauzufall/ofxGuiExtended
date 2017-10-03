#pragma once

#include "../ofxGuiElement.h"
#include "ofImage.h"

class ofxGuiFunctionPlotter : public ofxGuiElement {
	public:

		ofxGuiFunctionPlotter(ofParameter<ofPoint> value, const ofJson & config = ofJson());
		ofxGuiFunctionPlotter(ofParameter<ofPoint> value, std::function<float (float)> function, const ofJson & config = ofJson());

		virtual ~ofxGuiFunctionPlotter();

		virtual float getMinWidth() override;
		virtual float getMinHeight() override;

		void setPlotterStrokeWidth(float width);
		void setFunction(std::function<float (float)> function);

		virtual void setFillColor(const ofColor& color) override;
		virtual void setFillColor(const ofColor& minColor, const ofColor& maxColor);

		void setDecimalPlace(int place);

		virtual ofAbstractParameter & getParameter() override;

		static std::string getClassType();

	protected:

		void setup();

		virtual std::vector<std::string> getClassTypes() override;

		virtual void _setConfig(const ofJson & config) override;
		virtual void render() override;
		virtual void generateDraw() override;

		ofVboMesh textMesh;
		std::vector <float> buffer;
		ofPath plot;
		ofParameter<float> plotterStrokeWidth;
		ofParameter <std::string> label;
		ofParameter<ofPoint> value;
		std::function<float (float)> function;

		ofParameter<int> decimalPlace;
		ofColor minColor, maxColor;
		ofImage background_gradient;

		void valueChanged(ofPoint & value);

};
