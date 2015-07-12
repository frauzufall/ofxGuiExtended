#include "ofxGuiSpacer.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiSpacer::ofxGuiSpacer(string collectionName, float size, float x, float y){
    parent = NULL;
    setup(collectionName, size, x, y);
}

ofxGuiSpacer::ofxGuiSpacer(float size, float x, float y){
    parent = NULL;
    setup("", size, x, y);
}

ofxGuiSpacer * ofxGuiSpacer::setup(float size, float x, float y){
    return setup("", size, x, y);
}

ofxGuiSpacer * ofxGuiSpacer::setup(string collectionName, float size, float x, float y){
    this->setBackgroundColor(ofColor(0,0,0,0));
    this->setName(collectionName);
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

        if(parent->isAlignedVertial()) {
            b.width = parent->getWidth();
            b.height = spacing_size;
        }
        else {
            b.height = parent->getContentHeight();
            b.width = spacing_size;
        }

        parent->sizeChangedCB();
    }
    setNeedsRedraw();
}
