#include "ofxGuiValuePlotter.h"
#include "ofGraphics.h"
#include "../view/JsonConfigParser.h"
using namespace std;

ofxGuiValuePlotter::ofxGuiValuePlotter()
	:ofxGuiElement(){

	setup();

}

ofxGuiValuePlotter::ofxGuiValuePlotter(const ofJson & config)
	:ofxGuiElement(){

	setup();
	_setConfig(config);

}


ofxGuiValuePlotter::ofxGuiValuePlotter(ofParameter<float> &value, const ofJson & config) :
	ofxGuiElement(){

	this->value.makeReferenceTo(value);
	setup();
	_setConfig(config);
}

ofxGuiValuePlotter::ofxGuiValuePlotter(std::string label, float minValue, float maxValue, int plotSize, const ofJson & config)
	:ofxGuiValuePlotter(config){

	minVal = minValue;
	maxVal = maxValue;
	this->plotSize = plotSize;
	setName(label);

}

ofxGuiValuePlotter::~ofxGuiValuePlotter(){
	value.removeListener(this,&ofxGuiValuePlotter::valueChanged);
}

void ofxGuiValuePlotter::setup(){

	setTheme();

	decimalPlace.set("precision", 3);
	minVal.set("min", 0);
	maxVal.set("max", 0);
	plotSize.set("plotsize", 100);

	autoscale = minVal == maxVal;
	buffer.clear();
	value.addListener(this,&ofxGuiValuePlotter::valueChanged);

}

void ofxGuiValuePlotter::_setConfig(const ofJson & config){
	ofxGuiElement::_setConfig(config);
	// TODO
	JsonConfigParser::parse(config, decimalPlace);
}

float ofxGuiValuePlotter::getMinWidth(){
	float _width = ofxGuiElement::getTextWidth(ofToString(value.get(), decimalPlace));
	if(showName){
		_width += ofxGuiElement::getTextWidth(getName());
	}
	return _width;
}

float ofxGuiValuePlotter::getMinHeight(){
	return ofxGuiElement::getTextHeight(ofToString(value.get(), decimalPlace));
}

void ofxGuiValuePlotter::setDecimalPlace(int place){
	individualConfig[decimalPlace.getName()] = place;
	decimalPlace = place;
}

void ofxGuiValuePlotter::generateDraw(){

	ofxGuiElement::generateDraw();

	label = ofToString(value.get(), decimalPlace);
	if(showName){
		label += " " + this->getName();
	}

	textMesh = getTextMesh(label, textPadding, getShape().getHeight() / 2 + 4);

	if(plotSize > 0){

		plot.clear();
		if(minVal != maxVal && buffer.size() > 1){
			plot.moveTo(0, getShape().getHeight());
			unsigned int i;
			for(i = 0; i < buffer.size(); i++){
				float x = ofMap(i, 0, buffer.size() - 1, 0, getShape().getWidth());
				float y = ofMap(buffer[i], minVal, maxVal, getShape().getHeight(), 0);
				plot.lineTo(x, y);
			}
			plot.lineTo(getShape().getWidth(), getShape().getHeight());
			plot.close();
			plot.setFilled(true);
			plot.setFillColor(fillColor);
		}
	}
}

void ofxGuiValuePlotter::render(){
	ofColor c = ofGetStyle().color;

	ofxGuiElement::render();

	if(plotSize > 0){
		plot.draw();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(textColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

ofAbstractParameter & ofxGuiValuePlotter::getParameter(){
	return value;
}

void ofxGuiValuePlotter::valueChanged(float & value){
	if(plotSize > 0){
		buffer.push_back(value);

		if((int)buffer.size() > plotSize){
			buffer.erase(buffer.begin(), buffer.begin() + 1);
		}
		if(autoscale){
			if(value < minVal){
				minVal = value;
			}
			if(value > maxVal){
				maxVal = value;
			}
		}
	}
	setNeedsRedraw();
}

vector<std::string> ofxGuiValuePlotter::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}

std::string ofxGuiValuePlotter::getClassType(){
	return "value-plotter";
}

