#include "ofxToggle.h"
#include "ofxGuiGroup.h"
#include "ofGraphics.h"
using namespace std;

ofxToggle::ofxToggle():ofxBaseGui(){

	setup();

}

ofxToggle::ofxToggle(const string &toggleName):ofxToggle(){

	setName(toggleName);
	value.set(false);

}

ofxToggle::ofxToggle(const string &toggleName, const ofJson& config):ofxToggle(toggleName){

	_setConfig(config);

}

ofxToggle::ofxToggle(ofParameter<bool> &_bVal, const ofJson & config)
	:ofxBaseGui(){

	value.makeReferenceTo(_bVal);
	setup();
	_setConfig(config);

}

ofxToggle::ofxToggle(ofParameter<bool> &_bVal, float width, float height)
	:ofxToggle(_bVal){

	setSize(width, height);

}

ofxToggle::ofxToggle(const std::string& toggleName, bool _bVal, float width, float height)
	:ofxToggle(toggleName){

	value.set(_bVal);
	setSize(width, height);

}

ofxToggle::~ofxToggle(){

	value.removeListener(this,&ofxToggle::valueChanged);

}

void ofxToggle::setup(){

	hasFocus = false;

	type.set("type", ofxToggleType::CHECKBOX);

	setTheme();

	value.addListener(this,&ofxToggle::valueChanged);

	registerMouseEvents();

}

void ofxToggle::_setConfig(const ofJson &config){

	ofxBaseGui::_setConfig(config);

	if (config.find(type.getName()) != config.end()) {
		std::string val = config[type.getName()];
		setType(val);
	}

}

float ofxToggle::getMinWidth(){
	float _width = 0;
	if(showName){
		_width += ofxBaseGui::getTextWidth(getName()) + 2*textPadding;
	}
	if(type != ofxToggleType::FULLSIZE){
		_width += 30;
	}
	return _width;
}

float ofxToggle::getMinHeight(){
	float _height = 0;
	if(showName){
		_height += ofxBaseGui::getTextHeight(getName()) + 2*textPadding;
	}
	return _height;
}

bool ofxToggle::mousePressed(ofMouseEventArgs & args){
	ofxBaseGui::mousePressed(args);
	return setValue(args.x, args.y, true);
}

bool ofxToggle::mouseReleased(ofMouseEventArgs & args){
	ofxBaseGui::mouseReleased(args);
	bool hadFocus = hasFocus;
	hasFocus = false;
	return (hadFocus && isMouseOver());
}

void ofxToggle::generateDraw(){

	switch(type){
		default:
		case ofxToggleType::RADIO:
		case ofxToggleType::CHECKBOX: {
			checkboxRect.set(1, 1, getHeight() - 2, getHeight() - 2);
			break;
		}
		case ofxToggleType::FULLSIZE: {
			checkboxRect.set(1, 1, getWidth() - 2, getHeight() - 2);
			break;
		}
	}

	bg.clear();
	bg.setFilled(true);
	border.clear();
	border.setFillColor(borderColor);
	border.setFilled(true);
	if(value){
		bg.setFillColor(fillColor);
	}else{
		bg.setFillColor(backgroundColor);
	}
	switch(type){
		default:
		case ofxToggleType::RADIO:{
			border.arc(checkboxRect.getCenter(), checkboxRect.getHeight()/3, checkboxRect.getHeight()/3, 0, 360);
			if(value){
				bg.append(border);
			}else{
				bg.arc(checkboxRect.getCenter(), checkboxRect.getHeight()/3-borderWidth, checkboxRect.getHeight()/3-borderWidth, 0, 360);
				border.append(bg);
			}
			break;
		}
		case ofxToggleType::CHECKBOX: {
			border.rectangle(checkboxRect.getTopLeft()+ofPoint(checkboxRect.width/6,checkboxRect.height/6),
						 checkboxRect.width/3*2,checkboxRect.height/3*2);
			if(value){
				bg.append(border);
			}else{
				bg.rectangle(checkboxRect.getTopLeft()+ofPoint(checkboxRect.width/6+borderWidth,checkboxRect.height/6+borderWidth),
							 checkboxRect.width/3*2-2*borderWidth,checkboxRect.height/3*2 - 2*borderWidth);
				border.append(bg);
			}
			break;
		}
		case ofxToggleType::FULLSIZE: {
			border.rectangle(checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);
			if(value){
				bg.append(border);
			}else{
				bg.rectangle(checkboxRect.getTopLeft()+ofPoint(borderWidth,borderWidth),checkboxRect.width-2*borderWidth,checkboxRect.height-2*borderWidth);
				border.append(bg);
			}
			break;
		}
	}

	if(showName){
		switch(type){
			default:
			case ofxToggleType::RADIO:
			case ofxToggleType::CHECKBOX: {

	//			//create cross
	//			cross.clear();
	//			cross.setStrokeColor(textColor);
	//			cross.setStrokeWidth(1);
	//			cross.setFilled(false);
	//			cross.moveTo(checkboxRect.getTopLeft());
	//			cross.lineTo(checkboxRect.getBottomRight());
	//			cross.moveTo(checkboxRect.getTopRight());
	//			cross.lineTo(checkboxRect.getBottomLeft());

				// create label

				textMesh = getTextMesh(getName(), textPadding + checkboxRect.width, getShape().getHeight() / 2 + 4);
				break;
			}
			case ofxToggleType::FULLSIZE: {

				// create label
				float textWidth = ofxBaseGui::getTextWidth(getName());
				switch(textAlignment){
					default:
					case TextAlignment::CENTERED:
						if(getShape().getCenter().x - textWidth/2 > getShape().x+textPadding){
							textMesh = getTextMesh(getName(), getWidth()/2 - textWidth/2, getHeight()/ 2 + 4);
							break;
						}
					case TextAlignment::LEFT:
						textMesh = getTextMesh(getName(), textPadding, getShape().height / 2 + 4);
						break;
					case TextAlignment::RIGHT:
						textMesh = getTextMesh(getName(), getShape().getWidth() - textWidth - textPadding, getShape().height / 2 + 4);
						break;

				}
				break;
			}
		}
	}


}

void ofxToggle::render(){

	border.draw();
	bg.draw();

//	if(value && (type == ofxToggle::Type::CHECKBOX
//			|| type == ofxToggle::Type::RADIO)){
//		cross.draw();
//	}

	if(showName){
		ofColor c = ofGetStyle().color;
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
	}
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator const bool & (){
	return value;
}

bool ofxToggle::setValue(float mx, float my, bool bCheck){

	if(isHidden()){
		hasFocus = false;
		return false;
	}
	if(bCheck){

		ofRectangle checkRect = checkboxRect;
		checkRect.x += getScreenPosition().x;
		checkRect.y += getScreenPosition().y;

		hasFocus = checkRect.inside(mx, my);
	}

	if(hasFocus){

		//if group has exclusive toggles and toggle is on, don't to anything
		ofxGuiGroup* parent = dynamic_cast<ofxGuiGroup*>(this->parent());
		if(parent){
			if(parent->getTogglesExclusive()){
				if(value.get()){
					return false;
				}
			}
		}

		value = !value;

		if(value.get()){
			if(parent){
				parent->deactivateAllOtherToggles(this);
			}
		}

		setNeedsRedraw();

		return true;
	}

	return false;
}

ofAbstractParameter & ofxToggle::getParameter(){
	return value;
}

void ofxToggle::valueChanged(bool & value){
	setNeedsRedraw();
}

void ofxToggle::setType(const std::string& type){
	if(type == "checkbox"){
		setType(ofxToggleType::CHECKBOX);
	}
	else if(type == "radio"){
		setType(ofxToggleType::RADIO);
	}
	else if(type == "fullsize"){
		setType(ofxToggleType::FULLSIZE);
	}
}

void ofxToggle::setType(const ofxToggleType::Type &type){
	this->type.set(type);
	setNeedsRedraw();
}

ofxToggleType::Type ofxToggle::getType(){
	return type;
}

std::string ofxToggle::getClassType(){
	return "toggle";
}

vector<std::string> ofxToggle::getClassTypes(){
	vector<std::string> types = ofxBaseGui::getClassTypes();
	types.push_back(getClassType());
	return types;
}
