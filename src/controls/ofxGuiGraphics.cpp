#include "ofxGuiGraphics.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiGraphics::~ofxGuiGraphics(){
	ofRemoveListener(resize, this, &ofxGuiGraphics::onResize);
}

ofxGuiGraphics::ofxGuiGraphics(std::string canvasName, const ofJson& config)
	:ofxGuiElement(){
	_bLoaded = false;
	graphics = nullptr;
	setup(canvasName);
	_setConfig(config);
}

ofxGuiGraphics::ofxGuiGraphics(std::string canvasName, ofBaseDraws * graphics, const ofJson& config)
	:ofxGuiElement(){
	_bLoaded = false;
	setGraphics(graphics);
	setup(canvasName);
	_setConfig(config);
}

ofxGuiGraphics::ofxGuiGraphics(std::string canvasName, ofBaseDraws * graphics, float w, float h){
	_bLoaded = false;
	setGraphics(graphics);
	setup(canvasName,w,h);
}

void ofxGuiGraphics::setup(std::string canvasName, float w, float h){
	autoWidth = false;
	autoHeight = false;
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
	ofAddListener(resize, this, &ofxGuiGraphics::onResize);
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
	float _width = 0;
	if(showName){
		_width += ofxGuiElement::getTextWidth(getName()) + 2*textPadding;
	}
	return _width;
}

float ofxGuiGraphics::getMinHeight(){
	if(showName){
		return ofxGuiElement::getTextHeight(getName());
	}
	return 0;
}

void ofxGuiGraphics::setAutoHeight(){
	autoHeight = true;
	autoWidth = false;
	setHeight(getWidth() * graphics->getHeight() / graphics->getWidth());
}

void ofxGuiGraphics::setAutoWidth(){
	autoHeight = false;
	autoWidth = true;
	setWidth(getHeight() * graphics->getWidth() / graphics->getHeight());
}

void ofxGuiGraphics::onResize(DOM::ResizeEventArgs &args){
	if(autoHeight){
		setHeight(args.shape().width * graphics->getHeight() / graphics->getWidth());
	}
	if(autoWidth){
		setWidth(args.shape().height * graphics->getWidth() / graphics->getHeight());
	}
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

	if(_bLoaded && graphics){
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
