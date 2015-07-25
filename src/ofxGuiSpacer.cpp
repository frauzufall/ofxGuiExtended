#include "ofxGuiSpacer.h"
#include "ofxGuiGroupExtended.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiSpacer::ofxGuiSpacer(string name, float size, float x, float y){
    parent = NULL;
    setup(name, size, x, y);
}

ofxGuiSpacer::ofxGuiSpacer(float size, float x, float y){
    parent = NULL;
    setup("", size, x, y);
}

ofxGuiSpacer * ofxGuiSpacer::setup(float size, float x, float y){
    return setup("", size, x, y);
}

ofxGuiSpacer * ofxGuiSpacer::setup(string name, float size, float x, float y){
    this->setBackgroundColor(ofColor(0,0,0,0));
    this->setName(name);
    this->setPosition(x,y);
    spacing_size = size;
    return this;
}

void ofxGuiSpacer::generateDraw(){
    bg.clear();
    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(b.x,b.y,b.width,b.height);
}

void ofxGuiSpacer::render(){
    bg.draw();
}

void ofxGuiSpacer::sizeChangedCB(){

    if(parent) {

        if(ofxGuiGroupExtended* group = dynamic_cast<ofxGuiGroupExtended*>(parent)) {
            if(group->isAlignedVertical()) {
                b.width = group->getWidth();
                b.height = spacing_size;
            }
            else {
                b.height = group->getContentHeight();
                b.width = spacing_size;
            }
            parent->sizeChangedCB();
        }
    }
    setNeedsRedraw();
}

ofAbstractParameter & ofxGuiSpacer::getParameter(){
    return parameter;
}
