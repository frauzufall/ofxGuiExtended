#pragma once

#include "ofxGuiSlider.h"
#include "ofParameter.h"


template<typename DataType>
class ofxGuiFunctionSlider : public ofxGuiSlider<DataType> {
public:

	ofxGuiFunctionSlider();
	ofxGuiFunctionSlider(const ofJson & config);
	ofxGuiFunctionSlider(ofParameter<DataType>& _valOut, const ofJson & config = ofJson());

	~ofxGuiFunctionSlider();

	void setFunctions(std::function<DataType (DataType)> functionInToOut, std::function<DataType (DataType)> functionOutToIn);
	virtual bool mousePressed(ofMouseEventArgs & args) override;

protected:

	void setup();
	void valueInChanged(DataType & value);
	virtual void setValue(DataType value) override;
	virtual void generateDraw() override;
	virtual void setSliderBarValue(DataType value) override;
	virtual bool setValue(float mx, float my, bool bCheck) override;

	ofParameter<DataType> valueIn;
	std::function<DataType (DataType)> functionInToOut;
	std::function<DataType (DataType)> functionOutToIn;
};

typedef ofxGuiFunctionSlider<float> ofxGuiFloatFunctionSlider;
typedef ofxGuiFunctionSlider<int> ofxGuiIntFunctionSlider;
