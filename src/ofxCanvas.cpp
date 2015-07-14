#include "ofxCanvas.h"
#include "ofGraphics.h"
using namespace std;

ofxCanvas::ofxCanvas() {}

ofxCanvas::ofxCanvas(string canvasName, ofTexture *tex, float w, float h){
    setup(canvasName,tex,w,h);
}

ofxCanvas::ofxCanvas(ofTexture *tex, float w, float h){
    setup("",tex,w,h);
}

ofxCanvas::~ofxCanvas(){
}

ofxCanvas* ofxCanvas::setup(string canvasName, ofTexture *tex, float w, float h) {
    setName(canvasName);
    texture = tex;
    setSize(w,h);
    setNeedsRedraw();
    return this;
}

void ofxCanvas::setSize(float w, float h){
    if(texture != 0) {
        if(texture->isAllocated()) {
            if(w == 0) {
                if(h == 0) {
                    w = texture->getWidth();
                    h = texture->getHeight();
                }
                else {
                    w = h * texture->getWidth()/texture->getHeight();
                }
            }
            h = w*texture->getHeight()/texture->getWidth();
            b.width = w;
            b.height = h;
            setNeedsRedraw();
        }
    }
}

void ofxCanvas::generateDraw(){
    bg.clear();

    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(b);

    textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height-textPadding);
}

void ofxCanvas::render() {
    ofColor c = ofGetStyle().color;

    bg.draw();
    if(texture) {
        if(texture->isAllocated()) {
            texture->draw(b);
        }
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

ofAbstractParameter & ofxCanvas::getParameter(){
    return label;
}
