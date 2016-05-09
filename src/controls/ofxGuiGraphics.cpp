#include "ofxGuiGraphics.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiGraphics::~ofxGuiGraphics(){
//	ofRemoveListener(resize, this, &ofxGuiGraphics::onResize);
}

ofxGuiGraphics::ofxGuiGraphics(string canvasName, const ofJson& config)
	:ofxGuiElement(){
	_bLoaded = false;
	setup(canvasName);
	_setConfig(config);
}

ofxGuiGraphics::ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, const ofJson& config)
	:ofxGuiElement(){
	_bLoaded = false;
	setGraphics(graphics);
	setup(canvasName);
	_setConfig(config);
}

ofxGuiGraphics::ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, float w, float h){
	_bLoaded = false;
	setGraphics(graphics);
	setup(canvasName,w,h);
}

void ofxGuiGraphics::setup(string canvasName, float w, float h){
	setName(canvasName);
	if(_bLoaded){
		if(w == 0){
			if(h == 0){
				w = getWidth();
			}else{
				w = h * graphics->getWidth() / graphics->getHeight();
			}
		}
		h = w * graphics->getHeight() / graphics->getWidth();
		ofxGuiElement::setSize(w,h);
	}
	setTheme();
//	ofAddListener(resize, this, &ofxGuiGraphics::onResize);
}

void ofxGuiGraphics::setGraphics(ofBaseDraws *graphics){
	if(graphics){
		if(graphics->getHeight() != 0 && graphics->getWidth() != 0){
			_bLoaded = true;
			this->graphics = graphics;
		}else{
			ofLogWarning("ofxGuiGraphics:setGraphics()", "graphics cannot be loaded, width = 0 or height = 0");
		}
	}else{
		ofLogWarning("ofxGuiGraphics:setGraphics()", "graphics is nullptr");
	}
}

float ofxGuiGraphics::getMinWidth(){
	if(_bLoaded){
		return 10;
	}
	return 0;
}

float ofxGuiGraphics::getMinHeight(){
	if(_bLoaded){
		return 10;
	}
	return 0;
}

void ofxGuiGraphics::setAutoHeight(){
	setHeight(getWidth() * graphics->getHeight() / graphics->getWidth());
}

void ofxGuiGraphics::setAutoWidth(){
	setWidth(getHeight() * graphics->getWidth() / graphics->getHeight());
}

void ofxGuiGraphics::generateDraw(){
	ofxGuiElement::generateDraw();

	if(showName){
		textMesh = getTextMesh(getName(), textPadding, getHeight() - textPadding);
	}
}

void ofxGuiGraphics::render(){

	ofxGuiElement::render();

	ofColor c = ofGetStyle().color;

	if(_bLoaded){
		graphics->draw(0, 0, getWidth(), getHeight());
	}

	if(showName){
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
}

ofAbstractParameter & ofxGuiGraphics::getParameter(){
	return label;
}

std::string ofxGuiGraphics::getClassType(){
	return "graphics";
}

vector<std::string> ofxGuiGraphics::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}
