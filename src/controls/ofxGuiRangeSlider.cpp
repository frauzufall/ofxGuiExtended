#include "ofxGuiRangeSlider.h"
#include "JsonConfigParser.h"
#include "ofGraphics.h"
using namespace std;


template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider()
	:ofxGuiSlider<DataType>(){

	setup();

}

template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider(const ofJson &config)
	:ofxGuiRangeSlider(){

	this->_setConfig(config);

}

template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider(ofParameter<DataType>& _valStart, ofParameter<DataType> &_valEnd, const ofJson &config)
:ofxGuiSlider<DataType>(){

	valueStart.makeReferenceTo(_valStart);
	valueEnd.makeReferenceTo(_valEnd);
	setup();
	valueStart.addListener(this,&ofxGuiRangeSlider::valueStartChanged);
	valueEnd.addListener(this,&ofxGuiRangeSlider::valueEndChanged);
	this->_setConfig(config);

}

template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider(const std::string& sliderName, DataType _valStart, DataType _valEnd, DataType _min, DataType _max, const ofJson &config)
	:ofxGuiRangeSlider(config){

	valueStart.set(sliderName,_valStart,_min,_valEnd);
	valueEnd.set(sliderName,_valEnd,_valStart,_max);
	valueStart.addListener(this,&ofxGuiRangeSlider::valueStartChanged);
	valueEnd.addListener(this,&ofxGuiRangeSlider::valueEndChanged);

}

template<typename DataType>
ofxGuiRangeSlider<DataType>::~ofxGuiRangeSlider(){

	valueStart.removeListener(this,&ofxGuiRangeSlider::valueStartChanged);
	valueEnd.removeListener(this,&ofxGuiRangeSlider::valueEndChanged);

}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::setup(){

	activeValue = nullptr;

	values.add(valueStart);
	values.add(valueEnd);
	values.setName(valueStart.getName());

	valueEnd.setMin(valueStart.get());
	valueEnd.setMax(valueStart.getMax());
	valueStart.setMax(valueEnd.get());

	this->hasFocus = false;
	this->showValue.set("show-value", true);
	this->updateOnReleaseOnly.set("update-on-release-only", false);
	this->precision.set("precision", 6);
	this->horizontal = this->getWidth() > this->getHeight();

	this->setTheme();

	this->registerMouseEvents();

}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::setMin(DataType min){
	valueStart.setMin(min);
}

template<typename DataType>
DataType ofxGuiRangeSlider<DataType>::getMin(){
	return valueStart.getMin();
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::setMax(DataType max){
	valueEnd.setMax(max);
}

template<typename DataType>
DataType ofxGuiRangeSlider<DataType>::getMax(){
	return valueEnd.getMax();
}

template<typename DataType>
bool ofxGuiRangeSlider<DataType>::mouseReleased(ofMouseEventArgs & args){

	bool res = ofxGuiSlider<DataType>::mouseReleased(args);

	activeValue = nullptr;

	return res;

}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::generateDraw(){

	ofxGuiSlider<DataType>::generateDraw();

	this->bar.clear();
	float valStartAsPct, valEndAsPct;
	if(this->horizontal){
		valStartAsPct = ofMap(valueStart, getMin(), getMax(), 0, this->getWidth()-this->borderWidth*2, true);
		valEndAsPct = ofMap(valueEnd, getMin(), getMax(), 0, this->getWidth()-this->borderWidth*2, true);
	}else{
		valStartAsPct = ofMap(valueStart, getMin(), getMax(), 0, this->getHeight()-this->borderWidth*2, true);
		valEndAsPct = ofMap(valueEnd, getMin(), getMax(), 0, this->getHeight()-this->borderWidth*2, true);
	}
	if(valStartAsPct-valEndAsPct<0.01){
		valEndAsPct++;
	}
	this->bar.setFillColor(this->fillColor);
	this->bar.setFilled(true);
	if(this->horizontal){
		this->bar.rectRounded(this->borderWidth+valStartAsPct,this->borderWidth, valEndAsPct-valStartAsPct, this->getHeight()-this->borderWidth*2, this->borderRadius);
	}else{
		this->bar.rectRounded(this->borderWidth, this->getHeight() - valEndAsPct-this->borderWidth, this->getWidth()-this->borderWidth*2, valEndAsPct-valStartAsPct, this->borderRadius);
	}

	generateText();
}


template<typename DataType>
void ofxGuiRangeSlider<DataType>::generateText(){

	string valStr = ofToString(valueStart.get(), this->precision) + " - "
			+ ofToString(valueEnd.get(), this->precision);
	this->_generateText(valStr);
}

template<>
void ofxGuiRangeSlider<unsigned char>::generateText(){

	string valStr = ofToString((int)valueStart, precision) + " - "
			+ ofToString((int)valueEnd, precision);
	this->_generateText(valStr);
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::_generateText(std::string valStr){

	if(this->horizontal){
		this->textMesh.clear();
		if(this->showName){
			this->textMesh.append(this->getTextMesh(this->getName(), ofPoint(this->textPadding, this->getHeight() / 2 + 4)));
		}
		if(this->showValue){
			this->textMesh.append(this->getTextMesh(valStr, this->getShape().getWidth() - this->textPadding
										- this->getTextBoundingBox(valStr,0,0).width, this->getHeight() / 2 + 4));
		}
	}else{
		this->textMesh.clear();
		if(this->showName){
			string nameStr = this->getName();
			while(this->getTextBoundingBox(nameStr, 0, 0).getWidth() + this->textPadding * 2 > this->getWidth() && nameStr.length() > 1){
				nameStr = nameStr.substr(0, nameStr.size() - 1);
			}
			this->textMesh.append(this->getTextMesh(nameStr, this->textPadding, this->textPadding + this->getTextBoundingBox(nameStr, 0, 0).height));
		}
		if(this->showValue){
			while(this->getTextBoundingBox(valStr, 0, 0).getWidth() + this->textPadding * 2 > this->getWidth() && valStr.length() > 1){
				valStr = valStr.substr(0, valStr.size() - 1);
			}
			this->textMesh.append(this->getTextMesh(valStr, this->textPadding, this->getHeight() - this->textPadding));
		}
	}
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::render(){
	ofColor c = ofGetStyle().color;

	ofxGuiElement::render();

	this->bar.draw();

//	if(showName){
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(this->textColor);

		this->bindFontTexture();
		this->textMesh.draw();
		this->unbindFontTexture();

		ofSetColor(c);
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
//	}
}


template<typename DataType>
bool ofxGuiRangeSlider<DataType>::setValue(float mx, float my, bool bCheck){

	if(this->isHidden()){
		this->hasFocus = false;
		return false;
	}

	if(bCheck){
		this->hasFocus = this->isMouseOver();
	}

	if(this->hasFocus){

		ofPoint topleft = this->localToScreen(ofPoint(0, 0));
		ofPoint bottomright = this->localToScreen(ofPoint(this->getWidth(), this->getHeight()));

		if(!activeValue){
			DataType valStartX = ofMap(valueStart, getMin(), getMax(), topleft.x, bottomright.x, true);
			DataType valEndX = ofMap(valueEnd, getMin(), getMax(), topleft.x, bottomright.x, true);
			if(abs(valStartX-mx) > abs(valEndX-mx)){
				//closer to end marker
				activeValue = &valueEnd;
			}else{
				if(abs(valStartX-mx) < abs(valEndX-mx)){
					//closer to start marker
					activeValue = &valueStart;
				}else{
					if(mx > valStartX){
						activeValue = &valueEnd;
					}else {
						activeValue = &valueStart;
					}
				}
			}
		}

		DataType newval;
		if(this->horizontal){
			newval = ofMap(mx, topleft.x, bottomright.x, getMin(), getMax(), true);
		}else{
			newval = ofMap(my, bottomright.y, topleft.y, getMin(), getMax(), true);
		}
		if(newval > activeValue->getMax()){
			newval = activeValue->getMax();
		}else{
			if(newval < activeValue->getMin()){
				newval = activeValue->getMin();
			}
		}
		activeValue->set(newval);
		return true;

	}

	return false;
}

template<typename DataType>
ofAbstractParameter & ofxGuiRangeSlider<DataType>::getParameter(){
	return values;
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::valueStartChanged(DataType & value){
	valueEnd.setMin(value);
	this->setNeedsRedraw();
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::valueEndChanged(DataType & value){
	valueStart.setMax(value);
	this->setNeedsRedraw();
}

template<typename DataType>
std::string ofxGuiRangeSlider<DataType>::getText(){

	string res = "";
	if(this->showName){
		res += this->getName();
	}
	res += ofToString(valueStart.get(), this->precision);
	res += " - ";
	res += ofToString(valueEnd.get(), this->precision);

	return res;

}

template<>
std::string ofxGuiRangeSlider<unsigned char>::getText(){

	string res = "";
	if(showName){
		res += getName();
	}
	res += ofToString((int)valueStart, precision);
	res += " - ";
	res += ofToString((int)valueEnd, precision);

	return res;
}

template<typename DataType>
std::string ofxGuiRangeSlider<DataType>::getClassType(){
	return "rangeslider";
}

template<typename DataType>
vector<std::string> ofxGuiRangeSlider<DataType>::getClassTypes(){
	vector<std::string> types = ofxGuiSlider<DataType>::getClassTypes();
	types.push_back(getClassType());
	return types;
}

template class ofxGuiRangeSlider<int8_t>;
template class ofxGuiRangeSlider<uint8_t>;
template class ofxGuiRangeSlider<int16_t>;
template class ofxGuiRangeSlider<uint16_t>;
template class ofxGuiRangeSlider<int32_t>;
template class ofxGuiRangeSlider<uint32_t>;
template class ofxGuiRangeSlider<int64_t>;
template class ofxGuiRangeSlider<uint64_t>;
template class ofxGuiRangeSlider<float>;
template class ofxGuiRangeSlider<double>;
