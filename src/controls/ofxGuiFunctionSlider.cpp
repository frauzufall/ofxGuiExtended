#include "ofxGuiFunctionSlider.h"
#include "JsonConfigParser.h"
#include "ofGraphics.h"
using namespace std;


template<typename DataType>
ofxGuiFunctionSlider<DataType>::ofxGuiFunctionSlider()
	:ofxGuiSlider<DataType>(){
}

template<typename DataType>
ofxGuiFunctionSlider<DataType>::ofxGuiFunctionSlider(const ofJson &config)
	:ofxGuiSlider<DataType>(config){
	setup();
}

template<typename DataType>
ofxGuiFunctionSlider<DataType>::ofxGuiFunctionSlider(ofParameter<DataType>& _valOut, const ofJson &config)
:ofxGuiSlider<DataType>(_valOut, config){
	setup();

}

template<typename DataType>
ofxGuiFunctionSlider<DataType>::~ofxGuiFunctionSlider(){
	valueIn.removeListener(this,&ofxGuiFunctionSlider::valueInChanged);
}

template<typename DataType>
void ofxGuiFunctionSlider<DataType>::setup(){
	functionInToOut = [&] (DataType x) { return x; };
	functionOutToIn = [&] (DataType x) { return x; };
	valueIn.removeListener(this,&ofxGuiFunctionSlider::valueInChanged);
	valueIn.set(this->value.getName() + " _input", this->value.get(), this->value.getMin(), this->value.getMax());
}


template<typename DataType>
void ofxGuiFunctionSlider<DataType>::setValue(DataType value) {
	this->value.set(value);
	this->valueIn.set(functionOutToIn(value));
}

template<typename DataType>
bool ofxGuiFunctionSlider<DataType>::mousePressed(ofMouseEventArgs & args){

	ofxGuiElement::mousePressed(args);

	this->handleMousePressed(args.x, args.y, &this->valueIn);

	if(this->updateOnReleaseOnly){
		this->value.disableEvents();
	}
	return this->setValue(args.x, args.y, true);

}

template<typename DataType>
void ofxGuiFunctionSlider<DataType>::generateDraw(){
	this->generateShapes(&this->valueIn);
	this->generateText();
}

template<typename DataType>
void ofxGuiFunctionSlider<DataType>::setSliderBarValue(DataType value) {
	valueIn.set(value);
	this->value.set(functionInToOut(value));
}

template<typename DataType>
bool ofxGuiFunctionSlider<DataType>::setValue(float mx, float my, bool bCheck){
	return ofxGuiSlider<DataType>::setValue(mx, my, bCheck, &valueIn);
}

template<typename DataType>
void ofxGuiFunctionSlider<DataType>::valueInChanged(DataType & value){
	this->value.set(functionInToOut(value));
	this->setNeedsRedraw();
}

template<typename DataType>
void ofxGuiFunctionSlider<DataType>::setFunctions(std::function<DataType (DataType)> functionInToOut, std::function<DataType (DataType)> functionOutToIn) {
	this->functionInToOut = functionInToOut;
	this->functionOutToIn = functionOutToIn;
	valueIn.setMin(functionOutToIn(this->value.getMin()));
	valueIn.setMax(functionOutToIn(this->value.getMax()));
	valueIn.set(functionOutToIn(this->value));
	this->setNeedsRedraw();
}

template class ofxGuiFunctionSlider<int8_t>;
template class ofxGuiFunctionSlider<uint8_t>;
template class ofxGuiFunctionSlider<int16_t>;
template class ofxGuiFunctionSlider<uint16_t>;
template class ofxGuiFunctionSlider<int32_t>;
template class ofxGuiFunctionSlider<uint32_t>;
template class ofxGuiFunctionSlider<int64_t>;
template class ofxGuiFunctionSlider<uint64_t>;
template class ofxGuiFunctionSlider<float>;
template class ofxGuiFunctionSlider<double>;
