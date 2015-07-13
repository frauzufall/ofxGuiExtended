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
    b.width = max(element->getShape().getRight()-b.x+1, b.width);
    b.height = max(element->getShape().getBottom()-b.y+1, b.height);
    element->unregisterMouseEvents();

    parameters.add(element->getParameter());
    setNeedsRedraw();
}

bool ofxGuiPage::mouseDragged(ofMouseEventArgs & args){
    if(setValue(args.x, args.y, false)){
        return true;
    }
    if( bGuiActive ){
        ofMouseEventArgs a = args;
        for(int i = 0; i < (int)collection.size(); i++){
            if(collection[i]->mouseDragged(a)) {
                //collection is only allowed to be moved within page boundaries
                float tmp_header = 0;
                if(_bUseHeader) {
                    tmp_header += header + spacing;
                }
                collection[i]->setPosition(
                            ofClamp(
                                collection[i]->getPosition().x,
                                b.getLeft(),
                                b.getRight()-collection[i]->getWidth()),
                            ofClamp(
                                collection[i]->getPosition().y,
                                b.getTop()+tmp_header,
                                b.getBottom()-collection[i]->getHeight())
                            );
                return true;
            }
        }
    }
    return false;
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
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}
