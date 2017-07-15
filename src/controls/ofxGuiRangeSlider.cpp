#include "ofxGuiRangeSlider.h"
#include "JsonConfigParser.h"
#include "ofGraphics.h"
using namespace std;


template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider()
	:ofxGuiElement(){

	setup();

}

template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider(const ofJson &config)
	:ofxGuiRangeSlider(){

	_setConfig(config);

}

template<typename DataType>
ofxGuiRangeSlider<DataType>::ofxGuiRangeSlider(ofParameter<DataType>& _valStart, ofParameter<DataType> &_valEnd, const ofJson &config)
:ofxGuiElement(){

	valueStart.makeReferenceTo(_valStart);
	valueEnd.makeReferenceTo(_valEnd);
	setup();
	valueStart.addListener(this,&ofxGuiRangeSlider::valueStartChanged);
	valueEnd.addListener(this,&ofxGuiRangeSlider::valueEndChanged);
	_setConfig(config);

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
	ofRemoveListener(resize, this, &ofxGuiRangeSlider<DataType>::resized);

}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::setup(){

	activeValue = nullptr;

	values.add(valueStart);
	values.add(valueEnd);

	valueEnd.setMin(valueStart.get());
	valueEnd.setMax(valueStart.getMax());
	valueStart.setMax(valueEnd.get());

	hasFocus = false;
	showValue.set("show-value", true);
	updateOnReleaseOnly.set("update-on-release-only", false);
	precision.set("precision", 6);
	horizontal = getWidth() > getHeight();

	setTheme();

	ofAddListener(resize, this, &ofxGuiRangeSlider<DataType>::resized);
	registerMouseEvents();

}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::_setConfig(const ofJson &config){

	ofxGuiElement::_setConfig(config);

	JsonConfigParser::parse(config, updateOnReleaseOnly);
	JsonConfigParser::parse(config, precision);
	JsonConfigParser::parse(config, showValue);

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
void ofxGuiRangeSlider<DataType>::resized(DOM::ResizeEventArgs &){
	horizontal = getWidth() > getHeight();
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::setPrecision(int precision){
	individualConfig[this->precision.getName()] = precision;
	this->precision = precision;
}

template<typename DataType>
bool ofxGuiRangeSlider<DataType>::mousePressed(ofMouseEventArgs & args){

	ofxGuiElement::mousePressed(args);

	if(updateOnReleaseOnly){
		valueStart.disableEvents();
		valueEnd.disableEvents();
	}
	return setValue(args.x, args.y, true);

}

template<typename DataType>
bool ofxGuiRangeSlider<DataType>::mouseDragged(ofMouseEventArgs & args){

	ofxGuiElement::mouseDragged(args);

	return setValue(args.x, args.y, false);

}

template<typename DataType>
bool ofxGuiRangeSlider<DataType>::mouseReleased(ofMouseEventArgs & args){

	ofxGuiElement::mouseReleased(args);

	if(updateOnReleaseOnly){
		valueStart.enableEvents();
		valueEnd.enableEvents();
	}
	bool attended = setValue(args.x, args.y, false);
	activeValue = nullptr;
	hasFocus = false;
	return attended;

}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::generateDraw(){

	horizontal = getWidth() > getHeight();

	ofxGuiElement::generateDraw();

	bar.clear();

	float valStartAsPct, valEndAsPct;
	if(horizontal){
		valStartAsPct = ofMap(valueStart, getMin(), getMax(), 0, getWidth()-borderWidth*2, true);
		valEndAsPct = ofMap(valueEnd, getMin(), getMax(), 0, getWidth()-borderWidth*2, true);
	}else{
		valStartAsPct = ofMap(valueStart, getMin(), getMax(), 0, getHeight()-borderWidth*2, true);
		valEndAsPct = ofMap(valueEnd, getMin(), getMax(), 0, getHeight()-borderWidth*2, true);
	}
	bar.setFillColor(fillColor);
	bar.setFilled(true);
	if(horizontal){
		bar.rectRounded(borderWidth+valStartAsPct,borderWidth, valEndAsPct-valStartAsPct, getHeight()-borderWidth*2, borderRadius);
	}else{
		bar.rectRounded(borderWidth, getHeight() - valEndAsPct-borderWidth, getWidth()-borderWidth*2, valEndAsPct-valStartAsPct, borderRadius);
	}

	generateText();
}


template<typename DataType>
void ofxGuiRangeSlider<DataType>::generateText(){

	string valStr = ofToString(valueStart.get(), precision) + " - "
			+ ofToString(valueEnd.get(), precision);
	_generateText(valStr);
}

template<>
void ofxGuiRangeSlider<unsigned char>::generateText(){

	string valStr = ofToString((int)valueStart, precision) + " - "
			+ ofToString((int)valueEnd, precision);
	_generateText(valStr);
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::_generateText(std::string valStr){

	if(horizontal){
		textMesh.clear();
		if(showName){
			textMesh.append(getTextMesh(getName(), ofPoint(textPadding, getHeight() / 2 + 4)));
		}
		if(showValue){
			textMesh.append(getTextMesh(valStr, getShape().getWidth() - textPadding - getTextBoundingBox(valStr,0,0).width, getHeight() / 2 + 4));
		}
	}else{
		textMesh.clear();
		if(showName){
			string nameStr = getName();
			while(getTextBoundingBox(nameStr, 0, 0).getWidth() + textPadding * 2 > getWidth() && nameStr.length() > 1){
				nameStr = nameStr.substr(0, nameStr.size() - 1);
			}
			textMesh.append(getTextMesh(nameStr, textPadding, textPadding + getTextBoundingBox(nameStr, 0, 0).height));
		}
		if(showValue){
			while(getTextBoundingBox(valStr, 0, 0).getWidth() + textPadding * 2 > getWidth() && valStr.length() > 1){
				valStr = valStr.substr(0, valStr.size() - 1);
			}
			textMesh.append(getTextMesh(valStr, textPadding, getHeight() - textPadding));
		}
	}
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::render(){
	ofColor c = ofGetStyle().color;

	ofxGuiElement::render();

	bar.draw();

//	if(showName){
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(textColor);

		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
//	}
}


template<typename DataType>
bool ofxGuiRangeSlider<DataType>::setValue(float mx, float my, bool bCheck){

	if(isHidden()){
		hasFocus = false;
		return false;
	}

	if(bCheck){
		hasFocus = isMouseOver();
	}

	if(hasFocus){

		ofPoint topleft = localToScreen(ofPoint(0, 0));
		ofPoint bottomright = localToScreen(ofPoint(getWidth(), getHeight()));

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
		if(horizontal){
			newval = ofMap(mx, topleft.x, bottomright.x, getMin(), getMax(), true);
		}else{
			newval = ofMap(my, bottomright.y, topleft.y, getMin(), getMax(), true);
		}
		if(newval <= activeValue->getMax() && newval >= activeValue->getMin()){
			activeValue->set(newval);
		}
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
	setNeedsRedraw();
}

template<typename DataType>
void ofxGuiRangeSlider<DataType>::valueEndChanged(DataType & value){
	valueStart.setMax(value);
	setNeedsRedraw();
}

template<typename DataType>
std::string ofxGuiRangeSlider<DataType>::getText(){

	string res = "";
	if(showName){
		res += getName();
	}
	res += ofToString(valueStart.get(), precision);
	res += " - ";
	res += ofToString(valueEnd.get(), precision);

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
float ofxGuiRangeSlider<DataType>::getMinWidth(){
	return ofxGuiElement::getTextWidth(getText());
}

template<typename DataType>
float ofxGuiRangeSlider<DataType>::getMinHeight(){
	return ofxGuiElement::getTextHeight(getText());
}

template<typename DataType>
std::string ofxGuiRangeSlider<DataType>::getClassType(){
	return "rangeslider";
}

template<typename DataType>
vector<std::string> ofxGuiRangeSlider<DataType>::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
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
