#include "ofxGuiLabel.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxGuiValueLabel<Type>::ofxGuiValueLabel()
:ofxGuiElement(){

	setup();

}

template<typename Type>
ofxGuiValueLabel<Type>::ofxGuiValueLabel(ofParameter<Type>& _label, const ofJson & config)
:ofxGuiElement(){

	label.makeReferenceTo(_label);
	setup();
	_setConfig(config);

}

template<>
ofxGuiValueLabel<std::string>::ofxGuiValueLabel(const string& labelName, const ofJson & config)
:ofxGuiValueLabel<std::string>(){

	label.set(labelName);
	_setConfig(config);

}


template<typename Type>
ofxGuiValueLabel<Type>::ofxGuiValueLabel(ofParameter<Type>& _label, float width, float height)
	:ofxGuiValueLabel<Type>(_label){

	setSize(width, height);

}

template<typename Type>
ofxGuiValueLabel<Type>::ofxGuiValueLabel(const string& labelName, const Type& _label, const ofJson & config)
	:ofxGuiValueLabel<Type>(){

	label.set(labelName,_label);
	_setConfig(config);

}

template<typename Type>
ofxGuiValueLabel<Type>::ofxGuiValueLabel(const string& labelName, const Type& _label, float width, float height)
	:ofxGuiValueLabel<Type>(){

	label.set(labelName,_label);
	setSize(width, height);

}

template<typename Type>
ofxGuiValueLabel<Type>::~ofxGuiValueLabel(){

	label.removeListener(this,&ofxGuiValueLabel::valueChanged);

}

template<typename Type>
void ofxGuiValueLabel<Type>::setup(){

	label.setSerializable(false);
	setTheme();

	label.addListener(this,&ofxGuiValueLabel::valueChanged);

}

template<typename Type>
float ofxGuiValueLabel<Type>::getMinWidth(){
	string text = "";
	if(showName){
		if(!getName().empty()){
			text += getName();
		}
		if(!getName().empty() && label.toString() != ""){
			text += ": ";
		}
	}
	text += label.toString();

	return ofxGuiElement::getTextWidth(text);
}

template<typename Type>
float ofxGuiValueLabel<Type>::getMinHeight(){
	string text = "";
	if(showName){
		if(!getName().empty()){
			text += getName();
		}
		if(!getName().empty() && label.toString() != ""){
			text += ": ";
		}
	}
	text += label.toString();

	return ofxGuiElement::getTextHeight(text);
}

template<typename Type>
void ofxGuiValueLabel<Type>::generateDraw(){

	ofxGuiElement::generateDraw();

	renderedName = "";
	if(showName){
		if(!getName().empty()){
			renderedName += getName();
		}
		if(!getName().empty() && label.toString() != ""){
			renderedName += ": ";
		}
	}
	renderedName += label.toString();
#ifndef USE_FONTSTASH
	textMesh = getTextMesh(renderedName, textPadding, getShape().height / 2 + 4);
#endif
}

template<typename Type>
void ofxGuiValueLabel<Type>::render() {
	ofColor c = ofGetStyle().color;

	ofxGuiElement::render();

#ifndef USE_FONTSTASH

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
#else
	font.drawString(renderedName, textPadding, getShape().height / 2 + 4);
#endif

}

template<typename Type>
ofAbstractParameter & ofxGuiValueLabel<Type>::getParameter(){
	return label;
}

template<typename Type>
void ofxGuiValueLabel<Type>::valueChanged(Type & value){
	setNeedsRedraw();
}

template<typename Type>
std::string ofxGuiValueLabel<Type>::getClassType(){
	return "label";
}

template<typename Type>
vector<std::string> ofxGuiValueLabel<Type>::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}

template class ofxGuiValueLabel<std::string>;
template class ofxGuiValueLabel<int>;
template class ofxGuiValueLabel<float>;
template class ofxGuiValueLabel<bool>;
