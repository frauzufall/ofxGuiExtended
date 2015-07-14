#include "ofxCanvas.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
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
    if(tex != 0) {
        if(h == 0) {
            if(w != 0) {
                h = w * tex->getHeight()/tex->getWidth();
            }
            else {
                h = tex->getHeight();
            }
        }
        if(w == 0) {
            w = h * tex->getWidth()/tex->getHeight();
        }
    }
    this->setName(canvasName);
    b.width  = w;
    b.height = h;
    texture = tex;
    setNeedsRedraw();
    return this;
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
    if(texture)
        texture->draw(b);

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
