#include "ofxGuiSlider.h"
#include "JsonConfigParser.h"
#include "ofGraphics.h"
using namespace std;


template<typename DataType>
ofxGuiSlider<DataType>::ofxGuiSlider()
	:ofxGuiElement(){
}

template<typename DataType>
ofxGuiSlider<DataType>::ofxGuiSlider(const ofJson &config)
	:ofxGuiSlider(){

	setup();
	_setConfig(config);

}

template<typename DataType>
ofxGuiSlider<DataType>::ofxGuiSlider(ofParameter<DataType>& _val, const ofJson &config)
:ofxGuiElement(){

	value.makeReferenceTo(_val);
	value.addListener(this,&ofxGuiSlider::valueChanged);
	setup();
	_setConfig(config);

}

template<typename DataType>
ofxGuiSlider<DataType>::ofxGuiSlider(const std::string& sliderName, DataType _val, DataType _min, DataType _max, const ofJson &config)
	:ofxGuiSlider(config){

	value.set(sliderName,_val,_min,_max);
	value.addListener(this,&ofxGuiSlider::valueChanged);

}

template<typename DataType>
ofxGuiSlider<DataType>::~ofxGuiSlider(){

	value.removeListener(this,&ofxGuiSlider::valueChanged);
	ofRemoveListener(resize, this, &ofxGuiSlider<DataType>::resized);

}

template<typename DataType>
void ofxGuiSlider<DataType>::setup(){

	hasFocus = false;
	showValue.set("show-value", true);
	updateOnReleaseOnly.set("update-on-release-only", false);
	precision.set("precision", 6);
	type.set("type", ofxGuiSliderType::STRAIGHT);
	horizontal = getWidth() > getHeight();

	setTheme();

	ofAddListener(resize, this, &ofxGuiSlider<DataType>::resized);
	registerMouseEvents();

}

template<typename DataType>
void ofxGuiSlider<DataType>::_setConfig(const ofJson &config){

	ofxGuiElement::_setConfig(config);

	JsonConfigParser::parse(config, updateOnReleaseOnly);
	JsonConfigParser::parse(config, precision);
	JsonConfigParser::parse(config, showValue);

	if (config.find(type.getName()) != config.end()) {
		std::string val = config[type.getName()];
		setType(val);
	}


}

template<typename DataType>
void ofxGuiSlider<DataType>::setMin(DataType min){
	value.setMin(min);
}

template<typename DataType>
DataType ofxGuiSlider<DataType>::getMin(){
	return value.getMin();
}

template<typename DataType>
void ofxGuiSlider<DataType>::setMax(DataType max){
	value.setMax(max);
}

template<typename DataType>
DataType ofxGuiSlider<DataType>::getMax(){
	return value.getMax();
}

template<typename DataType>
void ofxGuiSlider<DataType>::setType(const std::string& type){
	if(type == "circular"){
		setType(ofxGuiSliderType::CIRCULAR);
	}
	else{
		setType(ofxGuiSliderType::STRAIGHT);
	}
}

template<typename DataType>
void ofxGuiSlider<DataType>::setType(const ofxGuiSliderType::Type& type){
	this->type.set(type);
	setNeedsRedraw();
}

template<typename DataType>
ofxGuiSliderType::Type ofxGuiSlider<DataType>::getType(){
	return type;
}


template<typename DataType>
void ofxGuiSlider<DataType>::resized(DOM::ResizeEventArgs &){
	horizontal = getWidth() > getHeight();
}

template<typename DataType>
void ofxGuiSlider<DataType>::setPrecision(int precision){
	individualConfig[this->precision.getName()] = precision;
	this->precision = precision;
}

template<typename DataType>
bool ofxGuiSlider<DataType>::mousePressed(ofMouseEventArgs & args){

	ofxGuiElement::mousePressed(args);

	handleMousePressed(args.x, args.y, &value);

	if(updateOnReleaseOnly){
		value.disableEvents();
	}
	return setValue(args.x, args.y, true);

}

template<typename DataType>
void ofxGuiSlider<DataType>::handleMousePressed(float x, float y, ofParameter<DataType>* referenceValue) {
	if((type == ofxGuiSliderType::CIRCULAR) && isMouseOver()){

		ofPoint pos = screenToLocal(ofPoint(x, y));

		DataType firstClickVal = ofMap(pos.y, getShape().getHeight(), 0, 0, 1, true);
		DataType lastVal = ofMap(referenceValue->get(), referenceValue->getMin(), referenceValue->getMax(), 0, 1, true);
		_mouseOffset = (firstClickVal - lastVal) * getShape().height;

	}
}

template<typename DataType>
bool ofxGuiSlider<DataType>::mouseDragged(ofMouseEventArgs & args){

	ofxGuiElement::mouseDragged(args);

	return setValue(args.x, args.y, false);

}

template<typename DataType>
bool ofxGuiSlider<DataType>::mouseReleased(ofMouseEventArgs & args){

	ofxGuiElement::mouseReleased(args);

	if(updateOnReleaseOnly){
		value.enableEvents();
	}
	bool attended = setValue(args.x, args.y, false);
	hasFocus = false;
	return attended;

}

template<typename DataType>
typename std::enable_if<std::is_integral<DataType>::value, DataType>::type
getRange(DataType min, DataType max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename DataType>
typename std::enable_if<std::is_floating_point<DataType>::value, DataType>::type
getRange(DataType min, DataType max, float width){
	double range = max - min;
	range /= width*4;
	return range;
}

template<typename DataType>
bool ofxGuiSlider<DataType>::mouseScrolled(ofMouseEventArgs & args){

	ofxGuiElement::mouseScrolled(args);

	if(isMouseOver()){
		if(args.scrollY>0 || args.scrollY<0){
			double range = getRange(value.getMin(),value.getMax(), getWidth());
			DataType newValue = value + ofMap(args.scrollY,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			setValue(newValue);
		}
		return true;
	}else{
		return false;
	}
}

template<typename DataType>
void ofxGuiSlider<DataType>::setValue(DataType value) {
	this->value.set(value);
}

template<typename DataType>
double ofxGuiSlider<DataType>::operator=(DataType v){
	setValue(v);
	return v;
}

template<typename DataType>
ofxGuiSlider<DataType>::operator const DataType & (){
	return value;
}

template<typename DataType>
void ofxGuiSlider<DataType>::generateDraw(){
	generateShapes(&value);
	generateText();
}

template<typename DataType>
void ofxGuiSlider<DataType>::generateShapes(ofParameter<DataType>* valueReference) {
	if(type == ofxGuiSliderType::STRAIGHT){

		horizontal = getWidth() > getHeight();

		ofxGuiElement::generateDraw();

		bar.clear();

		float valAsPct;
		if(horizontal){
			valAsPct = ofMap(valueReference->get(), valueReference->getMin(), valueReference->getMax(), 0, getWidth()-borderWidth*2, true);
		}else{
			valAsPct = ofMap(valueReference->get(), valueReference->getMin(), valueReference->getMax(), 0, getHeight()-borderWidth*2, true);
		}
		bar.setFillColor(fillColor);
		bar.setFilled(true);
		if(horizontal){
			bar.rectRounded(borderWidth,borderWidth, valAsPct, getHeight()-borderWidth*2, borderRadius);
		}else{
			bar.rectRounded(borderWidth, getHeight() - valAsPct-borderWidth, getWidth()-borderWidth*2, valAsPct, borderRadius);
		}

	}
	if(type == ofxGuiSliderType::CIRCULAR){

		ofPoint center = ofPoint(getWidth()/2, getHeight()/2);
		if(showName){
			center.y -= getTextHeight(getName());
		}
		float radius = min(center.x, center.y)-borderWidth;
		float inner_r = radius / 3;
		float outer_r = radius-1;

		bg.clear();
		bar.clear();

		bg.setStrokeColor(borderColor);
		bg.setStrokeWidth(1);
		bg.setFillColor(backgroundColor);
		bg.setFilled(true);
		arcStrip(bg, center, outer_r-1, inner_r+1, 1);

		float val = ofMap(valueReference->get(), valueReference->getMin(), valueReference->getMax(), 0, 1);
		bar.setFillColor(fillColor);
		bar.setFilled(true);
		arcStrip(bar, center, outer_r - 1, inner_r + 1, val);

	}
}


template<typename DataType>
void ofxGuiSlider<DataType>::generateText(){

	string valStr = ofToString(value.get(), precision);
	_generateText(valStr);
}

template<>
void ofxGuiSlider<unsigned char>::generateText(){

	string valStr = ofToString((int)value, precision);
	_generateText(valStr);
}

template<typename DataType>
void ofxGuiSlider<DataType>::_generateText(std::string valStr){

	if(type == ofxGuiSliderType::STRAIGHT){

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
	if(type == ofxGuiSliderType::CIRCULAR){

		textMesh.clear();
		if(showName){
			textMesh.append(getTextMesh(getName(), textPadding, getShape().height - textPadding));
		}
		if(showValue){
			textMesh.append(getTextMesh(valStr, getShape().width - textPadding - getTextBoundingBox(valStr, 0, 0).width, getShape().height - textPadding));
		}

	}
}

template<typename DataType>
void ofxGuiSlider<DataType>::render(){
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
bool ofxGuiSlider<DataType>::setValue(float mx, float my, bool bCheck){
	return setValue(mx, my, bCheck, &value);
}

template<typename DataType>
bool ofxGuiSlider<DataType>::setValue(float mx, float my, bool bCheck, ofParameter<DataType>* valueReference) {

	if(isHidden()){
		hasFocus = false;
		return false;
	}

	if(bCheck){
		hasFocus = isMouseOver();
	}

	if(hasFocus){

		if(type == ofxGuiSliderType::STRAIGHT){

			ofPoint topleft = localToScreen(ofPoint(0, 0));
			ofPoint bottomright = localToScreen(ofPoint(getWidth(), getHeight()));
			if(horizontal){
				setSliderBarValue(ofMap(mx, topleft.x, bottomright.x, valueReference->getMin(), valueReference->getMax(), true));
			}else{
				setSliderBarValue(ofMap(my, bottomright.y, topleft.y, valueReference->getMin(), valueReference->getMax(), true));
			}
			return true;

		}
		if(type == ofxGuiSliderType::CIRCULAR){

			ofPoint pos = screenToLocal(ofPoint(mx,my));

			DataType res = ofMap(pos.y,
							 getHeight() - _mouseOffset,
							 - _mouseOffset,
							 valueReference->getMin(),
							 valueReference->getMax(),
							 true);
			setSliderBarValue(res);
			return true;

		}

	}

	return false;
}

template<typename DataType>
void ofxGuiSlider<DataType>::setSliderBarValue(DataType value) {
	this->value.set(value);
}

template<typename DataType>
ofAbstractParameter & ofxGuiSlider<DataType>::getParameter(){
	return value;
}

template<typename DataType>
void ofxGuiSlider<DataType>::valueChanged(DataType & value){
	setNeedsRedraw();
}

template<typename DataType>
std::string ofxGuiSlider<DataType>::getText(){

	string res = "";
	if(type == ofxGuiSliderType::STRAIGHT){
		if(showName){
			res += getName();
		}
		res += ofToString(value.get(), precision);
	}

	return res;

}

template<>
std::string ofxGuiSlider<unsigned char>::getText(){

	string res = "";
	if(type == ofxGuiSliderType::STRAIGHT){
		if(showName){
			res += getName();
		}
		res += ofToString((int)value, precision);
	}

	return res;
}

template<typename DataType>
float ofxGuiSlider<DataType>::getMinWidth(){
	return ofxGuiElement::getTextWidth(getText());
}

template<typename DataType>
float ofxGuiSlider<DataType>::getMinHeight(){
	return ofxGuiElement::getTextHeight(getText());
}

/*
 * adapted from ofxUI by Reza Ali (www.syedrezaali.com || syed.reza.ali@gmail.com || @rezaali)
 *
 */
template <typename DataType>
void ofxGuiSlider <DataType>::arcStrip(ofPath & path, ofPoint center, float outer_radius, float inner_radius, float percent){
	float theta = ofMap(percent, 0, 1, 0, 360.0, true);

	{
		float x = sin(-ofDegToRad(0));
		float y = cos(-ofDegToRad(0));
		path.moveTo(center.x + outer_radius * x, center.y + outer_radius * y);
	}

	for(int i = 0; i <= theta; i += 10){
		float x = sin(-ofDegToRad(i));
		float y = cos(-ofDegToRad(i));

		path.lineTo(center.x + outer_radius * x, center.y + outer_radius * y);
	}

	{
		float x = sin(-ofDegToRad(theta));
		float y = cos(-ofDegToRad(theta));
		path.lineTo(center.x + outer_radius * x, center.y + outer_radius * y);
		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	for(int i = theta; i >= 0; i -= 10){
		float x = sin(-ofDegToRad(i));
		float y = cos(-ofDegToRad(i));

		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	{
		float x = sin(-ofDegToRad(0));
		float y = cos(-ofDegToRad(0));
		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	path.close();
}

template<typename DataType>
std::string ofxGuiSlider<DataType>::getClassType(){
	return "slider";
}

template<typename DataType>
vector<std::string> ofxGuiSlider<DataType>::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}

template class ofxGuiSlider<int8_t>;
template class ofxGuiSlider<uint8_t>;
template class ofxGuiSlider<int16_t>;
template class ofxGuiSlider<uint16_t>;
template class ofxGuiSlider<int32_t>;
template class ofxGuiSlider<uint32_t>;
template class ofxGuiSlider<int64_t>;
template class ofxGuiSlider<uint64_t>;
template class ofxGuiSlider<float>;
template class ofxGuiSlider<double>;
