#include "ofxGuiExtended.h"
#include "ofxGuiSliderGroup.h"
#include "../view/JsonConfigParser.h"
using namespace std;

template<class VecType>
ofxGuiVecSlider_<VecType>::ofxGuiVecSlider_()
	:ofxGuiGroup(){

	setup();

}

template<class VecType>
ofxGuiVecSlider_<VecType>::ofxGuiVecSlider_(const ofJson &config)
	:ofxGuiVecSlider_(){

	_setConfig(config);

}

template<class VecType>
ofxGuiVecSlider_<VecType>::ofxGuiVecSlider_(ofParameter<VecType> &value, const ofJson & config)
:ofxGuiVecSlider_(){

	setName(value.getName());

	names.clear();
	names.push_back("x");
	names.push_back("y");
	names.push_back("z");
	names.push_back("w");

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxGuiVecSlider_::changeValue);

	VecType val = value;
	VecType min = value.getMin();
	VecType max = value.getMax();
	for (int i=0; i<VecType::DIM; i++) {
		ofParameter<float> p(names[i], val[i], min[i], max[i]);
		add(p);
		p.addListener(this, & ofxGuiVecSlider_::changeSlider);
	}

	_setConfig(config);

}

template<class VecType>
ofxGuiVecSlider_<VecType>::ofxGuiVecSlider_(const std::string& controlName, const VecType & v, const VecType & min, const VecType & max, const ofJson & config)
	:ofxGuiVecSlider_(config){

	names.clear();
	names.push_back("x");
	names.push_back("y");
	names.push_back("z");
	names.push_back("w");

	value.set(controlName,v,min,max);

	this->value.addListener(this, & ofxGuiVecSlider_::changeValue);

	VecType val = value;
	for (int i=0; i<VecType::DIM; i++) {
		ofParameter<float> p(names[i], val[i], min[i], max[i]);
		add(p);
		p.addListener(this, & ofxGuiVecSlider_::changeSlider);
	}

}

template<class VecType>
ofxGuiVecSlider_<VecType>::~ofxGuiVecSlider_(){
	this->value.removeListener(this, & ofxGuiVecSlider_::changeValue);
	for (int i=0; i<VecType::DIM; i++) {
		getControl(names[i])->getParameter().template cast<float>().removeListener(this, &ofxGuiVecSlider_::changeSlider);
	}
}

template<class VecType>
void ofxGuiVecSlider_<VecType>::setup(){

	sliderChanging = false;

}

template<class VecType>
void ofxGuiVecSlider_<VecType>::changeSlider(const void * parameter, float & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName()) - getControlIndex(names[0]);
	VecType data = value;
	data[i] = _value;
	value = data;
	sliderChanging = false;
}

template<class VecType>
void ofxGuiVecSlider_<VecType>::changeValue(VecType & value){
	if (sliderChanging){
		return;
	}
	for (int i=0; i<VecType::DIM; i++){
		getControl(names[i])->getParameter().template cast<float>() = value[i];
	}
}

template<class VecType>
ofAbstractParameter & ofxGuiVecSlider_<VecType>::getParameter(){
	return value;
}

template<class VecType>
VecType ofxGuiVecSlider_<VecType>::operator=(const VecType & v){
	value = v;
	return value;
}

template<class VecType>
ofxGuiVecSlider_<VecType>::operator const VecType & (){
	return value;
}

template<class VecType>
const VecType * ofxGuiVecSlider_<VecType>::operator->(){
	return &value.get();
}

template class ofxGuiVecSlider_<ofVec2f>;
template class ofxGuiVecSlider_<ofVec3f>;
template class ofxGuiVecSlider_<ofVec4f>;


// RECTANGLE SLIDER

ofxGuiRectangleSlider::ofxGuiRectangleSlider()
	:ofxGuiGroup(){

	setup();

}

ofxGuiRectangleSlider::ofxGuiRectangleSlider(const ofJson &config)
	:ofxGuiRectangleSlider(){

	_setConfig(config);

}

ofxGuiRectangleSlider::ofxGuiRectangleSlider(ofParameter<ofRectangle> &value, const ofJson & config)
:ofxGuiRectangleSlider(){

	setName(value.getName());

	names.clear();
	names.push_back("x");
	names.push_back("y");
	names.push_back("width");
	names.push_back("height");

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxGuiRectangleSlider::changeValue);

	ofRectangle val = value;
	ofRectangle min = value.getMin();
	ofRectangle max = value.getMax();
	ofParameter<float> px(names[0], val.x, min.x, max.x);
	ofParameter<float> py(names[1], val.y, min.y, max.y);
	ofParameter<float> pw(names[2], val.width, min.width, max.width);
	ofParameter<float> ph(names[3], val.height, min.height, max.height);
	add(px);
	add(py);
	add(pw);
	add(ph);
	px.addListener(this, & ofxGuiRectangleSlider::changeSlider);
	py.addListener(this, & ofxGuiRectangleSlider::changeSlider);
	pw.addListener(this, & ofxGuiRectangleSlider::changeSlider);
	ph.addListener(this, & ofxGuiRectangleSlider::changeSlider);

	_setConfig(config);

}

ofxGuiRectangleSlider::ofxGuiRectangleSlider(const std::string& controlName, const ofRectangle & v, const ofRectangle & min, const ofRectangle & max, const ofJson & config)
	:ofxGuiRectangleSlider(config){

	names.clear();
	names.push_back("x");
	names.push_back("y");
	names.push_back("width");
	names.push_back("height");


	value.set(controlName,v,min,max);

	this->value.addListener(this, & ofxGuiRectangleSlider::changeValue);

	ofRectangle val = value;
	ofParameter<float> px(names[0], val.x, min.x, max.x);
	ofParameter<float> py(names[1], val.y, min.y, max.y);
	ofParameter<float> pw(names[2], val.width, min.width, max.width);
	ofParameter<float> ph(names[3], val.height, min.height, max.height);
	add(px);
	add(py);
	add(pw);
	add(ph);
	px.addListener(this, & ofxGuiRectangleSlider::changeSlider);
	py.addListener(this, & ofxGuiRectangleSlider::changeSlider);
	pw.addListener(this, & ofxGuiRectangleSlider::changeSlider);
	ph.addListener(this, & ofxGuiRectangleSlider::changeSlider);

}

ofxGuiRectangleSlider::~ofxGuiRectangleSlider(){
	this->value.removeListener(this, & ofxGuiRectangleSlider::changeValue);
	for (int i=0; i<4; i++){
		getControl(names[i])->getParameter().template cast<float>().removeListener(this, &ofxGuiRectangleSlider::changeSlider);
	}
}

void ofxGuiRectangleSlider::setup(){

	sliderChanging = false;

}

void ofxGuiRectangleSlider::changeSlider(const void * parameter, float & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName()) - getControlIndex(names[0]);
	ofRectangle data = value;
	switch(i){
		case 0: data.x = _value; break;
		case 1: data.y = _value; break;
		case 2: data.width = _value; break;
		case 3: data.height = _value; break;
	}
	value = data;
	sliderChanging = false;
}

void ofxGuiRectangleSlider::changeValue(ofRectangle & value){
	if (sliderChanging){
		return;
	}
	getControl("x")->getParameter().template cast<float>() = value.x;
	getControl("y")->getParameter().template cast<float>() = value.y;
	getControl("width")->getParameter().template cast<float>() = value.width;
	getControl("height")->getParameter().template cast<float>() = value.height;
}

ofAbstractParameter & ofxGuiRectangleSlider::getParameter(){
	return value;
}

ofRectangle ofxGuiRectangleSlider::operator=(const ofRectangle & v){
	value = v;
	return value;
}

ofxGuiRectangleSlider::operator const ofRectangle & (){
	return value;
}

const ofRectangle * ofxGuiRectangleSlider::operator->(){
	return &value.get();
}



// COLOR SLIDER


template<class ColorType>
ofxGuiColorSlider_<ColorType>::ofxGuiColorSlider_()
	:ofxGuiGroup(){

	setup();

}

template<class ColorType>
ofxGuiColorSlider_<ColorType>::ofxGuiColorSlider_(const ofJson &config)
	:ofxGuiGroup(){

	_setConfig(config);

}

template<class ColorType>
ofxGuiColorSlider_<ColorType>::ofxGuiColorSlider_(ofParameter<ofColor_<ColorType> > &value, const ofJson & config)
	:ofxGuiColorSlider_(){

	setName(value.getName());

	names.clear();
	names.push_back("r");
	names.push_back("g");
	names.push_back("b");
	names.push_back("a");

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxGuiColorSlider_::changeValue);

	ofColor_<ColorType> val = value;
	ofColor_<ColorType> min = value.getMin();
	ofColor_<ColorType> max = value.getMax();

	for (int i=0; i<4; i++) {
		ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
		add<ofxGuiSlider<ColorType>>(p);
		p.addListener(this, & ofxGuiColorSlider_::changeSlider);
		getControl(names[i])->setConfig(ofJson({
			{"fill-color", ofxGui::colorToString(value.get())}
		}));
	}

	sliderChanging = false;

	_setConfig(config);

}

template<class ColorType>
ofxGuiColorSlider_<ColorType>::ofxGuiColorSlider_(const std::string& controlName, const ofColor_<ColorType> & v, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, const ofJson &config)
	:ofxGuiColorSlider_(config){

	value.set(controlName,v,min,max);

	names.clear();
	names.push_back("r");
	names.push_back("g");
	names.push_back("b");
	names.push_back("a");

	this->value.addListener(this, & ofxGuiColorSlider_::changeValue);

	ofColor_<ColorType> val = value;

	for (int i=0; i<4; i++) {
		ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
		add<ofxGuiSlider<ColorType>>(p);
		p.addListener(this, & ofxGuiColorSlider_::changeSlider);
		getControl(names[i])->setConfig(ofJson({
			{"fill-color", ofxGui::colorToString(value.get())}
		}));
	}

	sliderChanging = false;

}


template<class ColorType>
ofxGuiColorSlider_<ColorType>::~ofxGuiColorSlider_(){
	this->value.removeListener(this, & ofxGuiColorSlider_::changeValue);
	for (int i=0; i<4; i++){
		getControl(names[i])->getParameter().template cast<ColorType>().removeListener(this, &ofxGuiColorSlider_::changeSlider);
	}
}


template<class ColorType>
void ofxGuiColorSlider_<ColorType>::setup(){

	sliderChanging = false;

}

template<class ColorType>
void ofxGuiColorSlider_<ColorType>::changeSlider(const void * parameter, ColorType & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName()) - getControlIndex(names[0]);
	ofColor_<ColorType> data = value;
	data[i] = _value;

	value = data;

	for (int i=0; i<4; i++){
		getControl(names[i])->setFillColor(value.get());
	}
	sliderChanging = false;
}

template<class ColorType>
void ofxGuiColorSlider_<ColorType>::changeValue(ofColor_<ColorType> & value){
	if (sliderChanging){
		return;
	}
	for (int i=0; i<4; i++){
		getControl(names[i])->getParameter().template cast<ColorType>() = value[i];
		getControl(names[i])->setFillColor(value);
	}
}

template<class ColorType>
ofAbstractParameter & ofxGuiColorSlider_<ColorType>::getParameter(){
	return value;
}

template<class ColorType>
ofColor_<ColorType> ofxGuiColorSlider_<ColorType>::operator=(const ofColor_<ColorType> & v){
	value = v;
	return value;
}

template<class ColorType>
ofxGuiColorSlider_<ColorType>::operator const ofColor_<ColorType> & (){
	return value;
}

template class ofxGuiColorSlider_<unsigned char>;
template class ofxGuiColorSlider_<unsigned short>;
template class ofxGuiColorSlider_<float>;
