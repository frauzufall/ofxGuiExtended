#include "ofxGuiGraphics.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiGraphics::~ofxGuiGraphics(){
}

ofxGuiGraphics::ofxGuiGraphics(string canvasName, const ofJson& config)
	:ofxBaseGui(config){
	_bLoaded = false;
	setName(canvasName);
}

ofxGuiGraphics::ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, const ofJson& config)
	:ofxBaseGui(){
	_bLoaded = false;
	setup(canvasName, graphics);
	_setConfig(config);
}

ofxGuiGraphics::ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, float w, float h){
	_bLoaded = false;
	setup(canvasName,graphics,w,h);
}

void ofxGuiGraphics::setup(string canvasName, ofBaseDraws * graphics, float w, float h){
	setName(canvasName);
	setGraphics(graphics);
	setSize(w,h);
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

void ofxGuiGraphics::setShape(float x, float y, float w, float h){
	ofxBaseGui::setPosition(x,y);
	setSize(w,h);
}

void ofxGuiGraphics::setShape(const ofRectangle &r){
	ofxBaseGui::setPosition(r.x,r.y);
	setSize(r.width,r.height);
}

void ofxGuiGraphics::setWidth(float w){
	if(graphics){
		float h = w * graphics->getHeight() / graphics->getWidth();
		ofxBaseGui::setSize(w,h);
	}else{
		ofxBaseGui::setWidth(w);
	}
}

void ofxGuiGraphics::setHeight(float h){
	if(graphics){
		float w = h * graphics->getWidth() / graphics->getHeight();
		ofxBaseGui::setSize(w,h);
	}else{
		ofxBaseGui::setHeight(h);
	}
}

void ofxGuiGraphics::setSize(float w, float h){
	if(_bLoaded){
		if(w == 0){
			if(h == 0){
				w = getWidth();
			}else{
				w = h * graphics->getWidth() / graphics->getHeight();
			}
		}
		h = w * graphics->getHeight() / graphics->getWidth();
		ofxBaseGui::setSize(w,h);
	}
}

void ofxGuiGraphics::generateDraw(){
	ofxBaseGui::generateDraw();

	if(showName){
		textMesh = getTextMesh(getName(), textPadding, getHeight() - textPadding);
	}
}

void ofxGuiGraphics::render(){

	ofxBaseGui::render();

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
