#include "ofxGuiButton.h"
using namespace std;

ofxGuiButton::ofxGuiButton()
	:ofxGuiToggle(){
	setup();
}

ofxGuiButton::ofxGuiButton(const string &buttonName)
	:ofxGuiButton(){
	value.setName(buttonName);
}

ofxGuiButton::ofxGuiButton(const string &buttonName, const ofJson & config)
	:ofxGuiButton(buttonName){
	_setConfig(config);
}

ofxGuiButton::ofxGuiButton(ofParameter<void> &_val, const ofJson & config)
	:ofxGuiToggle(){
	voidvalue.makeReferenceTo(_val);
	useVoidValue = true;
	setup();
	_setConfig(config);
}

ofxGuiButton::ofxGuiButton(ofParameter<bool> &_val, const ofJson & config)
	:ofxGuiToggle(_val){
	setup();
	_setConfig(config);
}

ofxGuiButton::~ofxGuiButton(){
}

void ofxGuiButton::setup(){

	value = false;
	value.setSerializable(false);
	voidvalue.setSerializable(false);
	registerMouseEvents();
	setTheme();

}

ofAbstractParameter& ofxGuiButton::getParameter(){
	if(useVoidValue){
		return voidvalue;
	}else {
		return value;
	}
}

bool ofxGuiButton::setValue(float mx, float my, bool bCheck){

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
		if(!value){
			voidvalue.trigger();
		}
		value = !value;
		return true;
	}
	return false;
}

void ofxGuiButton::generateDraw(){
	if(useVoidValue){
		value.setName(voidvalue.getName());
	}
	ofxGuiToggle::generateDraw();
}

bool ofxGuiButton::mouseReleased(ofMouseEventArgs & args){

	ofxGuiElement::mouseReleased(args);

	bool attended = setValue(args.x, args.y, false);
	hasFocus = false;
	return attended;

}

std::string ofxGuiButton::getClassType(){
	return "button";
}

vector<std::string> ofxGuiButton::getClassTypes(){
	vector<std::string> types = ofxGuiToggle::getClassTypes();
	types.push_back(getClassType());
	return types;
}
