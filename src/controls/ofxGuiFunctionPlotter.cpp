#include "ofxGuiFunctionPlotter.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiFunctionPlotter::ofxGuiFunctionPlotter(ofParameter<ofPoint> value, const ofJson & config) :
	ofxGuiElement(){

	this->function = [&](float x){return x;};
	this->value.makeReferenceTo(value);
	setup();
	_setConfig(config);
}

ofxGuiFunctionPlotter::ofxGuiFunctionPlotter(ofParameter<ofPoint> value, std::function<float (float)> function, const ofJson & config) :
	ofxGuiElement(){

	this->function = function;
	this->value.makeReferenceTo(value);
	setup();
	_setConfig(config);
}

ofxGuiFunctionPlotter::~ofxGuiFunctionPlotter(){
	value.removeListener(this,&ofxGuiFunctionPlotter::valueChanged);
}

void ofxGuiFunctionPlotter::setup(){

	plotterStrokeWidth.set("plotter-stroke-width", 1);
	decimalPlace.set("precision", 3);
	value.addListener(this,&ofxGuiFunctionPlotter::valueChanged);
	setNeedsRedraw();

}

void ofxGuiFunctionPlotter::_setConfig(const ofJson & config){
	ofxGuiElement::_setConfig(config);
	// TODO
}

float ofxGuiFunctionPlotter::getMinWidth(){
	float _width = 0;
	if(showName){
		_width += ofxGuiElement::getTextWidth(getName());
	}
	return _width;
}

float ofxGuiFunctionPlotter::getMinHeight(){
	if(showName){
		return ofxGuiElement::getTextHeight(getName());
	}
	return 0;
}

void ofxGuiFunctionPlotter::setDecimalPlace(int place){
	individualConfig[decimalPlace.getName()] = place;
	decimalPlace = place;
}

void ofxGuiFunctionPlotter::setFunction(std::function<float (float)> function){
	this->function = function;
	setNeedsRedraw();
}

void ofxGuiFunctionPlotter::generateDraw(){

	ofxGuiElement::generateDraw();

	textMesh = getTextMesh(this->getName(), textPadding, getShape().getHeight() / 2 + 4);

	plot.clear();
	plot.setFilled(false);
	plot.setStrokeWidth(plotterStrokeWidth);
	plot.setStrokeColor(ofColor::white);
	for(unsigned i = plotterStrokeWidth; i < getWidth()-plotterStrokeWidth; i++){
		float y_norm = ofMap(function(
								 ofMap(i,
									borderWidth, getWidth() - borderWidth*2,
									value.getMin().x, value.getMax().x)
							),
							value.getMin().y, value.getMax().y, 0, 1);
		if(i == plotterStrokeWidth){
			plot.moveTo(i, ofMap(
							y_norm,
							0, 1,
							borderWidth + plotterStrokeWidth/2, getHeight() - borderWidth - plotterStrokeWidth/2));
			continue;
		}
		plot.lineTo(i, ofMap(y_norm, 0, 1, borderWidth + plotterStrokeWidth/2, getHeight() - borderWidth - plotterStrokeWidth/2));
	}
}

void ofxGuiFunctionPlotter::render(){

	ofxGuiElement::render();

//	glEnable(GL_BLEND);
	plot.draw();
//	if(background_gradient.isAllocated()){
////		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glBlendEquation(GL_MIN);
//		background_gradient.draw(borderWidth,borderWidth,getWidth()-2*borderWidth, getHeight()-2*borderWidth);
//		ofEnableAlphaBlending();
//	}

	ofSetColor(textColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

}

ofAbstractParameter & ofxGuiFunctionPlotter::getParameter(){
	return value;
}

void ofxGuiFunctionPlotter::valueChanged(ofPoint & value){
	setNeedsRedraw();
}

void ofxGuiFunctionPlotter::setFillColor(const ofColor &color){
	setFillColor(color, color);
}

void ofxGuiFunctionPlotter::setFillColor(const ofColor &minColor, const ofColor &maxColor){
	this->minColor = minColor;
	this->maxColor = maxColor;
	if(background_gradient.isAllocated()){
		background_gradient.clear();
	}

	float w = 1;
	float h = getHeight();

	background_gradient.allocate(w, h, OF_IMAGE_COLOR);

	float r,g,b,a;
	ofColor c;

	for (float y=0; y<h; y++) {
		r = ofMap(y, 0, h, minColor.r, maxColor.r);
		g = ofMap(y, 0, h, minColor.g, maxColor.g);
		b = ofMap(y, 0, h, minColor.b, maxColor.b);
		a = ofMap(y, 0, h, minColor.a, maxColor.a);
		c = ofColor(r,g,b,a);
		for (float x=0; x<w; x++) {
			background_gradient.setColor(x,y,c);
		}
	}
	background_gradient.update();
	setNeedsRedraw();
}

void ofxGuiFunctionPlotter::setPlotterStrokeWidth(float width){
	individualConfig[plotterStrokeWidth.getName()] = width;
	plotterStrokeWidth.set(width);
}

vector<std::string> ofxGuiFunctionPlotter::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}

std::string ofxGuiFunctionPlotter::getClassType(){
	return "function-plotter";
}
