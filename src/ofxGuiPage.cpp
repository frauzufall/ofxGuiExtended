#include "ofxGuiPage.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiPage::ofxGuiPage():ofxPanelExtended(){
}

ofxGuiPage * ofxGuiPage::setup(string pageName, string filename, float x, float y){
    ofxPanelExtended::setup(pageName,filename,x,y);
    return this;
}

void ofxGuiPage::add(ofxBaseGui * element){
    collection.push_back( element );

    ofPoint newpos = element->getPosition()+this->getPosition();
    if(_bUseHeader) {
        newpos.y += header;
    }
    element->setPosition(newpos);
    element->unregisterMouseEvents();

    parameters.add(element->getParameter());
    setNeedsRedraw();
}

void ofxGuiPage::setWidthElements(float w){
}

void ofxGuiPage::scaleWidthElements(float factor){
}

void ofxGuiPage::clear(){
    collection.clear();
    parameters.clear();
    b.height = spacing + spacingNextElement ;
    if(_bUseHeader) {
        b.height += header;
    }
    sizeChangedCB();
}

void ofxGuiPage::sizeChangedCB(){
}
