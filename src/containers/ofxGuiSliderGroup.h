#pragma once

#include "ofxGuiGroup.h"


template<class VecType>
class ofxGuiVecSlider_ : public ofxGuiGroup {
public:

	ofxGuiVecSlider_();
	ofxGuiVecSlider_(const ofJson & config);
	ofxGuiVecSlider_(ofParameter<VecType> &value, const ofJson & config = ofJson());
	ofxGuiVecSlider_(const std::string& controlName, const VecType & value, const VecType & min, const VecType & max, const ofJson & config = ofJson());

	~ofxGuiVecSlider_();

	void setup();

	ofAbstractParameter & getParameter();

	VecType operator=(const VecType & v);
	operator const VecType & ();
	const VecType * operator->();
protected:
	void changeSlider(const void * parameter, float & value);
	void changeValue(VecType & value);
	ofParameter<VecType> value;
	bool sliderChanging;
	std::vector<std::string> names;
};

class ofxGuiRectangleSlider : public ofxGuiGroup {
public:

	ofxGuiRectangleSlider();
	ofxGuiRectangleSlider(const ofJson & config);
	ofxGuiRectangleSlider(ofParameter<ofRectangle> &value, const ofJson & config = ofJson());
	ofxGuiRectangleSlider(const std::string& controlName, const ofRectangle & value, const ofRectangle & min, const ofRectangle & max, const ofJson & config = ofJson());

	~ofxGuiRectangleSlider();

	void setup();

	ofAbstractParameter & getParameter();

	ofRectangle operator=(const ofRectangle & v);
	operator const ofRectangle & ();
	const ofRectangle * operator->();
protected:
	void changeSlider(const void * parameter, float & value);
	void changeValue(ofRectangle & value);
	ofParameter<ofRectangle> value;
	bool sliderChanging;
	std::vector<std::string> names;
};

template<typename ColorType>
class ofxGuiColorSlider_: public ofxGuiGroup{

public:

	ofxGuiColorSlider_();
	ofxGuiColorSlider_(const ofJson & config);
	ofxGuiColorSlider_(ofParameter<ofColor_<ColorType>> &value, const ofJson & config = ofJson());
	ofxGuiColorSlider_(const std::string& controlName, const ofColor_<ColorType> & value, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, const ofJson & config = ofJson());

	~ofxGuiColorSlider_();

	void setup();

	ofAbstractParameter & getParameter();

	ofColor_<ColorType> operator=(const ofColor_<ColorType> & v);
	operator const ofColor_<ColorType> & ();
protected:
	void changeSlider(const void * parameter, ColorType & value);
	void changeValue(ofColor_<ColorType> & value);
	ofParameter<ofColor_<ColorType> > value;
	bool sliderChanging;
	std::vector<std::string> names;
};

