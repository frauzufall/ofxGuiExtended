#include "ofxGuiMatrix.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiMatrix::ofxGuiMatrix(){
}

ofxGuiMatrix::ofxGuiMatrix(string collectionName, int cols, string filename, float x, float y)
    :ofxGuiGroupExtended(){
    setup(collectionName, cols, filename, x, y);
}

ofxGuiMatrix::ofxGuiMatrix(const ofParameterGroup & parameters, int cols, string filename, float x, float y)
    :ofxGuiGroupExtended(parameters, filename, x, y){
    setColNum(cols);
    sizeChangedCB();
    setNeedsRedraw();
}

ofxGuiMatrix * ofxGuiMatrix::setup(string collectionName, int cols, string filename, float x, float y){
    setColNum(cols);
    ofxGuiGroupExtended::setup(collectionName,filename,x,y);
    w_matrix = b.width;
    return this;
}

ofxGuiMatrix * ofxGuiMatrix::setup(const ofParameterGroup & _parameters, int cols, string filename, float x, float y){
    setColNum(cols);
    ofxGuiGroupExtended::setup(_parameters,filename,x,y);
    w_matrix = b.width;
    return this;
}

void ofxGuiMatrix::add(ofxBaseGui *element){
    collection.push_back( element );

    sizeChangedCB();

    element->unregisterMouseEvents();

    ofxGuiGroupExtended * subgroup = dynamic_cast<ofxGuiGroupExtended*>(element);
    if(subgroup!=NULL){
        subgroup->filename = filename;
        subgroup->parent = this;
        subgroup->scaleWidthElements(.98);
    }else{
        if(parent!=NULL){
            scaleWidthElements(0.98);
        }
    }

    parameters.add(element->getParameter());
    setNeedsRedraw();
}

void ofxGuiMatrix::setWidthElements(float w){
    if((int)b.width != (int)w) {
        scaleWidthElements(w / b.width);
    }
}

void ofxGuiMatrix::scaleWidthElements(float factor){

    w_matrix = this->b.getWidth()*factor;

    sizeChangedCB();
    setNeedsRedraw();

}

bool ofxGuiMatrix::mouseMoved(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseMoved(args);
}


bool ofxGuiMatrix::mousePressed(ofMouseEventArgs & args){
    if(setValue(args.x, args.y, true)){
        return true;
    }
    if( bGuiActive ){
        ofMouseEventArgs a = args;
        for(int i = 0; i < (int)collection.size(); i++){
            ofxToggle* toggle = dynamic_cast<ofxToggle*>(collection[i]);
            if(toggle && !_bAllowMultiple) {
                if(processToggles(toggle, a)) return true;
            }
            else {
                if(collection[i]->mousePressed(a)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ofxGuiMatrix::mouseDragged(ofMouseEventArgs & args){
    if(setValue(args.x, args.y, false)){
        return true;
    }
    if( bGuiActive ){
        ofMouseEventArgs a = args;
        for(int i = 0; i < (int)collection.size(); i++){
            if(collection[i]->mouseDragged(a)) return true;
        }
    }
    return false;
}

bool ofxGuiMatrix::mouseReleased(ofMouseEventArgs & args){
    return ofxGuiGroupExtended::mouseReleased(args);
}

bool ofxGuiMatrix::mouseScrolled(ofMouseEventArgs & args){
    return ofxGuiGroupExtended::mouseScrolled(args);
}

bool ofxGuiMatrix::setValue(float mx, float my, bool bCheck){

    if( !isGuiDrawing() ){
        bGuiActive = false;
        return false;
    }


    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;

            ofRectangle minButton(b.x+b.width-textPadding*3,b.y,textPadding*3,header);
            if(minButton.inside(mx,my)){
                minimized = !minimized;
                if(minimized){
                    minimize();
                }else{
                    maximize();
                }
                return true;
            }
        }
    }

    return false;
}

void ofxGuiMatrix::minimize(){
    minimized=true;
    b.height = spacing + spacingNextElement + 1 /*border*/;
    if(_bUseHeader) {
        b.height += header;
    }
    if(parent) {
        parent->sizeChangedCB();
    }
    setNeedsRedraw();
}

void ofxGuiMatrix::maximize(){
    minimized=false;
    b.height += (h_element+spacing)*getRowNum();
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiMatrix::minimizeAll(){
    for(int i=0;i<(int)collection.size();i++){
        ofxGuiMatrix * group = dynamic_cast<ofxGuiMatrix*>(collection[i]);
        if(group)group->minimize();
    }
}

void ofxGuiMatrix::maximizeAll(){
    for(int i=0;i<(int)collection.size();i++){
        ofxGuiMatrix * group = dynamic_cast<ofxGuiMatrix*>(collection[i]);
        if(group)group->maximize();
    }
}

void ofxGuiMatrix::sizeChangedCB(){
    updateElementWidth();
    float x,y;
    x = b.x;
    y = b.getBottom()-getContentHeight();

    x += b.width - w_matrix;

    for(unsigned int i = 0; i < collection.size(); i++) {
        int x_e = i;
        if(numCol > 0) {
            x_e = x_e % numCol;
        }
        int y_e = 0;
        if(numCol > 0) {
            y_e = (int)(i / numCol);
        }

        collection[i]->setSize(w_element, h_element);

        if(i == 0) {
            collection[i]->setPosition(x, y);
        }
        else {
            ofRectangle first_shape = collection[0]->getShape();
            collection[i]->setPosition(first_shape.x + (w_element + spacing)*x_e, first_shape.y + (h_element + spacing)*y_e);
        }

    }

    b.height = y + (h_element+spacing)*getRowNum()+spacing - b.y;

    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiMatrix::updateElementWidth() {
    w_element = w_matrix/numCol-spacing;
}

void ofxGuiMatrix::setColNum(int num) {
    numCol = num;
}

int ofxGuiMatrix::getColNum() {
    return numCol;
}

int ofxGuiMatrix::getRowNum() {
    if(collection.size() > 0) {
        return (int)((collection.size()-1)/numCol+1);
    }
    else {
        return 0;
    }

}

void ofxGuiMatrix::setElementHeight(float h) {
    h_element = h;
}

