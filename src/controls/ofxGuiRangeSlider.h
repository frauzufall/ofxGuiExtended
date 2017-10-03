#pragma once

#include "ofxGuiSlider.h"
#include "ofParameter.h"

template<typename DataType>
class ofxGuiRangeSlider : public ofxGuiSlider<DataType> {
public:

	ofxGuiRangeSlider();
	ofxGuiRangeSlider(const ofJson & config);
	ofxGuiRangeSlider(ofParameter<DataType>& _valStart, ofParameter<DataType>& _valEnd, const ofJson & config = ofJson());
	ofxGuiRangeSlider(const std::string& sliderName, DataType _valStart, DataType _valEnd, DataType _min, DataType _max, const ofJson & config = ofJson());

	~ofxGuiRangeSlider();

	void setMin(DataType min);
	DataType getMin();
	void setMax(DataType max);
	DataType getMax();

	virtual bool mouseReleased(ofMouseEventArgs & args) override;

	template<class ListenerClass, typename ListenerMethod>
	void addListenerStart(ListenerClass * listener, ListenerMethod method){
		valueStart.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void addListenerEnd(ListenerClass * listener, ListenerMethod method){
		valueEnd.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListenerStart(ListenerClass * listener, ListenerMethod method){
		valueStart.removeListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListenerEnd(ListenerClass * listener, ListenerMethod method){
		valueEnd.removeListener(listener,method);
	}

	ofAbstractParameter & getParameter() override;

	static std::string getClassType();

protected:

	void setup();

	virtual std::vector<std::string> getClassTypes() override;

	virtual void render() override;

	ofParameter<DataType> valueStart,valueEnd;
	ofParameterGroup values;
	virtual bool setValue(float mx, float my, bool bCheck) override;
	virtual void generateDraw() override;
	virtual void generateText() override;
	virtual void _generateText(std::string valStr) override;
	void valueStartChanged(DataType & value);
	void valueEndChanged(DataType & value);
	virtual std::string getText();

	ofParameter<DataType> *activeValue;

};

typedef ofxGuiRangeSlider<float> ofxGuiFloatRangeSlider;
typedef ofxGuiRangeSlider<int> ofxGuiIntRangeSlider;
