#include "ofxFunctionPlotter.h"
#include "ofGraphics.h"
using namespace std;

ofxFunctionPlotter::ofxFunctionPlotter(ofParameter<ofPoint> value, const ofJson & config) :
	ofxBaseGui(){

	this->function = [&](float x){return x;};
	this->value.makeReferenceTo(value);
	setup();
	_setConfig(config);
}

ofxFunctionPlotter::ofxFunctionPlotter(ofParameter<ofPoint> value, std::function<float (float)> function, const ofJson & config) :
	ofxBaseGui(){

	this->function = function;
	this->value.makeReferenceTo(value);
	setup();
	_setConfig(config);
}

ofxFunctionPlotter::~ofxFunctionPlotter(){
	value.removeListener(this,&ofxFunctionPlotter::valueChanged);
}

void ofxFunctionPlotter::setup(){

	plotterStrokeWidth.set("plotter-stroke-width", 1);
	decimalPlace.set("precision", 3);
	value.addListener(this,&ofxFunctionPlotter::valueChanged);
	setNeedsRedraw();

}

void ofxFunctionPlotter::_setConfig(const ofJson & config){
	ofxBaseGui::_setConfig(config);
	// TODO
}

void ofxFunctionPlotter::setDecimalPlace(int place){
	decimalPlace = place;
}

void ofxFunctionPlotter::setFunction(std::function<float (float)> function){
	this->function = function;
	setNeedsRedraw();
}

void ofxFunctionPlotter::generateDraw(){

	ofxBaseGui::generateDraw();

	textMesh = getTextMesh(this->getName(), textPadding, getShape().getHeight() / 2 + 4);

	plot.clear();
	plot.setFilled(false);
	plot.setStrokeWidth(plotterStrokeWidth);
	plot.setStrokeColor(ofColor::white);
	for(unsigned i = plotterStrokeWidth; i < getWidth()-plotterStrokeWidth; i++){
		float y_norm = ofMap(function(ofMap(i, 0, getWidth(), value.getMin().x, value.getMax().x)), value.getMin().y, value.getMax().y, 0, 1);
		if(i == plotterStrokeWidth){
			plot.moveTo(i, ofMap(y_norm, 0, 1, plotterStrokeWidth/2, getHeight()-plotterStrokeWidth/2));
			continue;
		}
		plot.lineTo(i, ofMap(y_norm, 0, 1, plotterStrokeWidth/2, getHeight()-plotterStrokeWidth/2));
	}
}

void ofxFunctionPlotter::render(){

	ofxBaseGui::render();

	plot.draw();
	if(background_gradient.isAllocated()){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_MIN);
		background_gradient.draw(0,0,getWidth(), getHeight());
		ofEnableAlphaBlending();
	}

}

ofAbstractParameter & ofxFunctionPlotter::getParameter(){
	return value;
}

void ofxFunctionPlotter::valueChanged(ofPoint & value){
	setNeedsRedraw();
}

void ofxFunctionPlotter::setFillColor(const ofColor &color){
	setFillColor(color, color);
}

void ofxFunctionPlotter::setFillColor(const ofColor &minColor, const ofColor &maxColor){
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

void ofxFunctionPlotter::setPlotterStrokeWidth(float width){
	plotterStrokeWidth.set(width);
}
