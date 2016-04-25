#include "ofxLabel.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel()
:ofxBaseGui(){

	setup();

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(ofParameter<Type> _label, const ofJson & config)
:ofxBaseGui(){

	label.makeReferenceTo(_label);
	setup();
	_setConfig(config);

}

template<>
ofxValueLabel<std::string>::ofxValueLabel(const string& labelName, const ofJson & config)
:ofxValueLabel<std::string>(){

	label.set(labelName);
	_setConfig(config);

}


template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(ofParameter<Type> _label, float width, float height)
	:ofxValueLabel<Type>(_label){

	setSize(width, height);

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(const string& labelName, const Type& _label, const ofJson & config)
	:ofxValueLabel<Type>(){

	label.set(labelName,_label);
	_setConfig(config);

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(const string& labelName, const Type& _label, float width, float height)
	:ofxValueLabel<Type>(){

	label.set(labelName,_label);
	setSize(width, height);

}

template<typename Type>
ofxValueLabel<Type>::~ofxValueLabel(){

	label.removeListener(this,&ofxValueLabel::valueChanged);

}

template<typename Type>
void ofxValueLabel<Type>::setup(){

	setTheme();
	label.addListener(this,&ofxValueLabel::valueChanged);

}

template<typename Type>
float ofxValueLabel<Type>::getMinWidth(){
	string text = "";
	if(showName){
		if(!getName().empty()){
			text = getName() + ": ";
		}
	}
	text += label.toString();

	return ofxBaseGui::getTextWidth(text)+2*textPadding;
}

template<typename Type>
float ofxValueLabel<Type>::getMinHeight(){
	string text = "";
	if(showName){
		if(!getName().empty()){
			text = getName() + ": ";
		}
	}
	text += label.toString();
	return ofxBaseGui::getTextHeight(text)+2*textPadding;
}

template<typename Type>
void ofxValueLabel<Type>::generateDraw(){

	ofxBaseGui::generateDraw();

	if(showName){
		string name;
		if(!getName().empty()){
			name = getName() + ": ";
		}
		textMesh = getTextMesh(name + label.toString(), textPadding, getShape().height / 2 + 4);
	}else {
		textMesh = getTextMesh(label.toString(), textPadding, getShape().height / 2 + 4);
	}
}

template<typename Type>
void ofxValueLabel<Type>::render() {
	ofColor c = ofGetStyle().color;

	ofxBaseGui::render();

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

template<typename Type>
ofAbstractParameter & ofxValueLabel<Type>::getParameter(){
	return label;
}

template<typename Type>
void ofxValueLabel<Type>::valueChanged(Type & value){
	setNeedsRedraw();
}

template<typename Type>
std::string ofxValueLabel<Type>::getClassType(){
	return "label";
}

template<typename Type>
vector<std::string> ofxValueLabel<Type>::getClassTypes(){
	vector<std::string> types = ofxBaseGui::getClassTypes();
	types.push_back(getClassType());
	return types;
}

template class ofxValueLabel<std::string>;
template class ofxValueLabel<int>;
template class ofxValueLabel<float>;
template class ofxValueLabel<bool>;
