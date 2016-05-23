#pragma once

#include "../ofxGuiElement.h"

class ofxGuiValuePlotter : public ofxGuiElement {
	public:

		ofxGuiValuePlotter();
		ofxGuiValuePlotter(const ofJson & config);
		ofxGuiValuePlotter(ofParameter<float>& value, const ofJson & config = ofJson());
		ofxGuiValuePlotter(string label, float minValue, float maxValue, int plotSize = 100, const ofJson & config = ofJson());

		virtual ~ofxGuiValuePlotter();

		virtual float getMinWidth() override;
		virtual float getMinHeight() override;

		void setDecimalPlace(int place);

		virtual ofAbstractParameter & getParameter() override;

		static std::string getClassType();

	protected:

		void setup();

		virtual vector<std::string> getClassTypes() override;

		virtual void _setConfig(const ofJson & config) override;
		virtual void render() override;
		virtual void generateDraw() override;

#ifndef USE_FONTSTASH
		ofVboMesh textMesh;
#endif
		vector <float> buffer;
		ofPath plot;
		bool autoscale;
		ofParameter <string> label;
		ofParameter<float> value;

		ofParameter<int> decimalPlace;
		ofParameter<float> minVal;
		ofParameter<float> maxVal;
		ofParameter<int> plotSize;

		void valueChanged(float & value);

};
