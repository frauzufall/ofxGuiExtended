#include "ofxGuiBaseDraws.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiBaseDraws::~ofxGuiBaseDraws(){
}

ofxGuiBaseDraws::ofxGuiBaseDraws(const Config & config){
	graphics = config.graphics;
    if(graphics && graphics->getHeight() != 0 && graphics->getWidth() != 0) {
        _bLoaded = true;
        setName(config.canvasName);
        this->graphics = graphics;
        setSize(config.shape.width,config.shape.height);
    }  else {
        ofLogError("ofxCanvas:setup()", "graphics cannot be loaded, width = 0 or height = 0");
    }
}

void ofxGuiBaseDraws::setSize(float w, float h){
    if(_bLoaded) {
        if(w == 0) {
            if(h == 0) {
                w = graphics->getWidth();
                h = graphics->getHeight();
            }
            else {
                w = h * graphics->getWidth()/graphics->getHeight();
            }
        }
        h = w*graphics->getHeight()/graphics->getWidth();
        b.width = w;
        b.height = h;
        setNeedsRedraw();
    }
}

void ofxGuiBaseDraws::generateDraw(){
    bg.clear();

    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(b);

    textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height-textPadding);
}

void ofxGuiBaseDraws::render() {
    ofColor c = ofGetStyle().color;

    bg.draw();
    if(_bLoaded) {
        graphics->draw(b);
    }


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

ofAbstractParameter & ofxGuiBaseDraws::getParameter(){
    return label;
}
