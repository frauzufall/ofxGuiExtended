#include "ofxTabbedPages.h"
#include "ofGraphics.h"
#include "ofxMinimalToggle.h"
using namespace std;

ofxTabbedPages::ofxTabbedPages()
:ofxGuiPage(){}

ofxTabbedPages::~ofxTabbedPages(){
    //
}

ofxTabbedPages * ofxTabbedPages::setup(string collectionName, string filename, float x, float y) {
    ofxGuiPage::setup(collectionName,filename,x,y);
    tabs.setup();
    tabs.setShowHeader(false);
    tabs.allowMultipleActiveToggles(false);
    tabs.setAlignHorizontal();
    tabs.setBorderColor(ofColor(0,0,0,0));
    tabs.setDefaultBackgroundColor(thisBorderColor);
    tabs.setDefaultBorderColor(thisBorderColor);
    tabs.setDefaultFillColor(thisBackgroundColor);
    tabHeight = 20;
    tabWidth = 0;
    updateContentShapes();
    sizeChangedCB();
    collection.push_back(&tabs);
    parameters.add(tabs.getParameter());
    return this;
}

void ofxTabbedPages::add(ofxGuiPage *element) {
    collection.push_back(element);

    parameters_tabs.push_back(ofParameter<bool>(element->getName(),false));
    tabs.add(new ofxMinimalToggle(parameters_tabs.at(parameters_tabs.size()-1),tabWidth,tabHeight));
    if(element->getBackgroundColor()!=thisBackgroundColor) {
        tabs.getControl(tabs.getNumControls()-1)->setBackgroundColor(element->getBackgroundColor());
        tabs.getControl(tabs.getNumControls()-1)->setBorderColor(element->getBackgroundColor());
    }

    element->setPosition(pagesShape.getPosition());
    element->setShowHeader(false);
    element->setBorderColor(ofColor(0,0,0,0));
    setSizeToElement(element);

    element->setSize(pagesShape.width, pagesShape.height);

    element->unregisterMouseEvents();

    parameters.add(element->getParameter());

    if(collection.size() == 2) {
        setActiveTab(0);
    }

    setNeedsRedraw();
}

void ofxTabbedPages::clear(){
    collection.resize(1);
    parameters.clear();
    parameters.add(tabs.getParameter());
    b.height = spacing + spacingNextElement ;
    if(_bUseHeader) {
        b.height += header;
    }
    tabs.clear();
    sizeChangedCB();
}

void ofxTabbedPages::generateDraw(){

    updateContentShapes();

    bg.clear();
    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(pagesShape.x, pagesShape.y, pagesShape.width, pagesShape.height);

    border.clear();
    border.setStrokeColor(thisBorderColor);
    border.setStrokeWidth(1);
    border.setFilled(false);
    border.rectangle(pagesShape.x-1, pagesShape.y+1, pagesShape.width+2, pagesShape.height+2);

    if(_bUseHeader) {
        generateHeader();
    }
}

void ofxTabbedPages::render(){

    tabBorder.clear();
    tabBorder.setStrokeWidth(2);
    tabBorder.setFilled(false);
    tabBorder.setStrokeColor(thisBackgroundColor);
    tabBorder.moveTo(activeToggle->getShape().getBottomLeft()-ofPoint(-1,1));
    tabBorder.lineTo(activeToggle->getShape().getBottomRight()-ofPoint(1,1));

    for(int i = 1; i < (int)collection.size(); i++){
        if(parameters_tabs.at(i-1).get()) {
            activePage = collection[i];
            activeToggle = tabs.getControl(i-1);
        }
    }

    this->setBorderColor(activeToggle->getBorderColor());

    bg.draw();
    tabs.draw();
    border.draw();
    tabBorder.draw();
    if(_bUseHeader) {
        headerBg.draw();
    }

    ofBlendMode blendMode = ofGetStyle().blendingMode;
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableAlphaBlending();
    }
    ofColor c = ofGetStyle().color;

    if(_bUseHeader) {
        renderHeader();
    }

    activePage->draw();

    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}

bool ofxTabbedPages::mouseDragged(ofMouseEventArgs & args){
    if(setValue(args.x, args.y, false)){
        return true;
    }
    if( bGuiActive ){
        ofMouseEventArgs a = args;
        for(int i = 0; i < (int)collection.size(); i++){
            if(i > 0) {
                if(parameters_tabs.at(i-1).get()) {
                    if(collection[i]->mouseDragged(a)) return true;
                }
            }
            else {
                if(collection[i]->mouseDragged(a)) return true;
            }

        }
    }
    return false;
}

bool ofxTabbedPages::mouseReleased(ofMouseEventArgs & args){
    this->bGrabbed = false;
    if(ofxGuiGroupExtended::mouseReleased(args)) return true;
    if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
        return true;
    }else{
        return false;
    }
}

bool ofxTabbedPages::setValue(float mx, float my, bool bCheck){

    if( !isGuiDrawing() ){
        bGrabbed = false;
        bGuiActive = false;
        return false;
    }
    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;

            if( my > b.y && my <= b.y + header ){
                bGrabbed = true;
                grabPt.set(mx-b.x, my-b.y);
            } else{
                bGrabbed = false;
            }

            if(loadBox.inside(mx, my)) {
                loadFromFile(filename);
                ofNotifyEvent(loadPressedE,this);
                return true;
            }
            if(saveBox.inside(mx, my)) {
                saveToFile(filename);
                ofNotifyEvent(savePressedE,this);
                return true;
            }
        }
    } else if( bGrabbed ){
        setPosition(mx - grabPt.x,my - grabPt.y);
        updateContentShapes();
        return true;
    }
    return false;
}

void ofxTabbedPages::updateContentShapes() {
    ofPoint newpos = b.getPosition();
    if(_bUseHeader) {
        newpos.y += header + spacing;
    }

    tabShape.setPosition(newpos);
    tabShape.setWidth(b.width);
    if(tabs.getNumControls() > 0) {
        tabShape.setHeight(tabHeight);
    }

    newpos.x += 1/*border*/;
    newpos.y += tabHeight;
    pagesShape.setPosition(newpos);
    pagesShape.setWidth(b.width-2/*border*/);
    pagesShape.setHeight(b.height - newpos.y + b.y - 1/*border*/);
}

void ofxTabbedPages::setSizeToElement(ofxBaseGui *element) {
    updateContentShapes();
    if(element->getWidth() > pagesShape.width) {
        b.width = element->getWidth()+2/*border*/;
    }
    if(element->getHeight() > pagesShape.getHeight()) {
        b.height = element->getHeight() + tabHeight + 1/*border*/;
        if(_bUseHeader) {
            b.height += spacing + header;
        }
    }
    updateContentShapes();
    sizeChangedCB();
}

void ofxTabbedPages::sizeChangedCB(){
    tabs.setPosition(tabShape.getPosition());
    if(tabs.getNumControls() > 0) {
        tabs.setSize(tabShape.width, tabShape.height);
    }
    else {
        tabs.setSize(tabShape.width, tabs.getHeight());
    }

    for(unsigned int i = 1; i < collection.size(); i++) {
        collection[i]->setShape(pagesShape);
    }
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxTabbedPages::setActiveTab(int index) {
    tabs.setActiveToggle(index);
    activeToggle = tabs.getControl(index);
}

int ofxTabbedPages::getActiveTabIndex() {
    return tabs.getActiveToggleIndex();
}

ofxGuiPage* ofxTabbedPages::getActiveTab() {
    return (ofxGuiPage*)collection.at(tabs.getActiveToggleIndex()+1);
}

void ofxTabbedPages::setTabHeight(int h) {
    tabHeight = h;
}

void ofxTabbedPages::setTabWidth(int w) {
    tabWidth = w;
}
