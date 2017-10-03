#pragma once

#include "../ofxGuiElement.h"
#include "ofParameter.h"

class ofxGuiSliderType{
	public:
	enum Type{
		/// \brief Default. Shows slider as a vertical or horizontal bar.
		STRAIGHT,
		/// \brief Displays circular slider.
		CIRCULAR
	};
};

template<typename DataType>
class ofxGuiSlider : public ofxGuiElement, public ofxGuiSliderType{
public:

	ofxGuiSlider();
	ofxGuiSlider(const ofJson & config);
	ofxGuiSlider(ofParameter<DataType>& _val, const ofJson & config = ofJson());
	ofxGuiSlider(const std::string& sliderName, DataType _val, DataType _min, DataType _max, const ofJson & config = ofJson());

	~ofxGuiSlider();

	void setMin(DataType min);
	DataType getMin();
	void setMax(DataType max);
	DataType getMax();

	void setType(const std::string &type);
	void setType(const Type &type);
	Type getType();

	virtual float getMinWidth() override;
	virtual float getMinHeight() override;

	void setPrecision(int precision);

	void setUpdateOnReleaseOnly(bool bUpdateOnReleaseOnly);

	virtual bool mousePressed(ofMouseEventArgs & args) override;
	virtual bool mouseDragged(ofMouseEventArgs & args) override;
	virtual bool mouseReleased(ofMouseEventArgs & args) override;
	virtual bool mouseScrolled(ofMouseEventArgs & args) override;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	double operator=(DataType v);
	operator const DataType & ();

	ofAbstractParameter & getParameter() override;

	static std::string getClassType();

protected:

	void setup();

	virtual std::vector<std::string> getClassTypes() override;

	virtual void _setConfig(const ofJson & config) override;
	virtual void render() override;

	virtual void resized(DOM::ResizeEventArgs&);

	ofParameter<DataType> value;
	ofParameter<ofxGuiSliderType::Type> type;
	virtual bool setValue(float mx, float my, bool bCheck) override;
	virtual void generateDraw() override;
	virtual void generateText();
	virtual void _generateText(std::string valStr);
	void valueChanged(DataType & value);
	virtual std::string getText();
	ofPath bar;
	ofVboMesh textMesh;

	ofParameter<bool> updateOnReleaseOnly;
	ofParameter<bool> showValue;
	ofParameter<unsigned int> precision;
	/// \brief The Slider orientation.
	bool horizontal;

	bool hasFocus;

	//circular type
	void arcStrip(ofPath & path, ofPoint center, float outer_radius, float inner_radius, float percent);
	float _mouseOffset;


};

typedef ofxGuiSlider<float> ofxGuiFloatSlider;
typedef ofxGuiSlider<int> ofxGuiIntSlider;
