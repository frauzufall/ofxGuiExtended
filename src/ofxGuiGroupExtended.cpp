#include "ofxGuiGroupExtended.h"
#include "ofGraphics.h"
#include "ofxGui.h"
using namespace std;

ofxGuiGroupExtended::ofxGuiGroupExtended(){
    _bVertical = true;
    _bUseHeader = true;
    _bAllowMultiple = true;
    active_toggle_index = -1;
}

ofxGuiGroupExtended::ofxGuiGroupExtended(const ofParameterGroup & parameters, string filename, float x, float y)
    :ofxGuiGroup(parameters, filename, x, y){
    _bVertical = true;
    _bUseHeader = true;
    _bAllowMultiple = true;
    active_toggle_index = -1;
}

ofxGuiGroupExtended * ofxGuiGroupExtended::setup(string collectionName, string filename, float x, float y){
    ofxGuiGroup::setup(collectionName,filename,x,y);
    clear();
    return this;
}

ofxGuiGroupExtended * ofxGuiGroupExtended::setup(const ofParameterGroup & _parameters, string filename, float x, float y){
    ofxGuiGroup::setup(_parameters,filename,x,y);
	for(int i=0;i<(int)collection.size();i++){
		collection[i]->setSize(this->getWidth()/collection.size()-spacingNextElement, collection[i]->getHeight());
	}
    return this;
}

void ofxGuiGroupExtended::add(ofxBaseGui * element){
    collection.push_back( element );

    if(_bVertical || collection.size() == 1) {

        if(_bVertical) {
            if(b.width-1 < element->getWidth()) {
                element->setSize(b.width-1, element->getHeight());
            }
            if(b.width-1 > element->getWidth()) {
                element->setSize(b.width-1, element->getHeight());
            }
        }

        element->setPosition(b.x, b.y + b.height  + spacing);

        b.height += element->getHeight() + spacing;

    }
    else {
        ofRectangle last_shape = collection[collection.size()-2]->getShape();
        element->setPosition(last_shape.x + last_shape.getWidth() + spacing, last_shape.y);
        b.width = element->getPosition().x + element->getWidth() + spacing - b.x;
        if(element->getHeight() > getContentHeight()) {
            setContentHeight(element->getHeight());
        }
    }

    element->unregisterMouseEvents();
    element->setParent(this);
    element->sizeChangedCB();

    ofxGuiGroupExtended * subgroup = dynamic_cast<ofxGuiGroupExtended*>(element);
    if(subgroup!=NULL){
        subgroup->filename = filename;
        subgroup->scaleWidthElements(.98);
    }else{
        if(parent!=NULL){
            element->setSize(b.width*.98,element->getHeight());
            element->setPosition(b.x + b.width-element->getWidth(),element->getPosition().y);
        }
    }

    parameters.add(element->getParameter());

    if(!_bAllowMultiple) {
        setOneToggleActive();
    }

    setNeedsRedraw();
}

void ofxGuiGroupExtended::setWidthElements(float w){
    if((int)b.width != (int)w) {
        scaleWidthElements(w / b.width);
    }
}

void ofxGuiGroupExtended::scaleWidthElements(float factor){

    float w = b.getWidth()*factor;

    if(_bVertical) {
        for(int i=0;i<(int)collection.size();i++){
            collection[i]->setSize(w,collection[i]->getHeight());
            collection[i]->setPosition(b.x + b.width-w,collection[i]->getPosition().y);
            ofxGuiGroupExtended * subgroup = dynamic_cast<ofxGuiGroupExtended*>(collection[i]);
            if(subgroup!=NULL){
                subgroup->scaleWidthElements(factor);
            }
        }
    }
    else {
        float x = spacing;
        for(int i=(int)collection.size()-1;i>=0;i--){
            float w_i = collection[i]->getWidth()*factor;
            collection[i]->setSize(w_i,collection[i]->getHeight());
            x += w_i+spacing;
            collection[i]->setPosition(b.x + b.width-x,collection[i]->getPosition().y);
            ofxGuiGroupExtended * subgroup = dynamic_cast<ofxGuiGroupExtended*>(collection[i]);
            if(subgroup!=NULL){
                subgroup->scaleWidthElements(factor);
            }
        }
    }

    sizeChangedCB();
    setNeedsRedraw();

}

void ofxGuiGroupExtended::clear(){
    collection.clear();
    parameters.clear();
    setContentHeight(0);
    sizeChangedCB();
    active_toggle_index = -1;
}

bool ofxGuiGroupExtended::mouseMoved(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseMoved(args);
}


bool ofxGuiGroupExtended::mousePressed(ofMouseEventArgs & args){
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

bool ofxGuiGroupExtended::processToggles(ofxToggle* toggle, ofMouseEventArgs a) {
    if(!_bAllowMultiple) {
        if(!toggle->getParameter().cast<bool>().get()) {
            if(toggle->mousePressed(a)) {
                deactivateAllOtherToggles(toggle);
                return true;
            }
        }
    }
    return false;
}

bool ofxGuiGroupExtended::mouseDragged(ofMouseEventArgs & args){
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

bool ofxGuiGroupExtended::mouseReleased(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseReleased(args);
}

bool ofxGuiGroupExtended::mouseScrolled(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseScrolled(args);
}

void ofxGuiGroupExtended::generateDraw(){
    border.clear();
    border.setFillColor(ofColor(thisBorderColor));
    border.setFilled(true);
    border.rectangle(b.x,b.y+ spacingNextElement,b.width+1,b.height);

    if(_bUseHeader) {
        headerBg.clear();
        headerBg.setFillColor(thisHeaderBackgroundColor);
        headerBg.setFilled(true);
        headerBg.rectangle(b.x,b.y + spacingNextElement, b.width+1, header);

        textMesh = getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y+ spacingNextElement);
        if(minimized){
            textMesh.append(getTextMesh("+", b.width-textPadding-8 + b.x, header / 2 + 4+ b.y+ spacingNextElement));
        }else{
            textMesh.append(getTextMesh("-", b.width-textPadding-8 + b.x, header / 2 + 4 + b.y+ spacingNextElement));
        }
    }

}

void ofxGuiGroupExtended::render(){
    border.draw();
    if(_bUseHeader) {
        headerBg.draw();
    }

    ofBlendMode blendMode = ofGetStyle().blendingMode;
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableAlphaBlending();
    }
    ofColor c = ofGetStyle().color;

    if(_bUseHeader) {
        ofSetColor(thisTextColor);

        bindFontTexture();
        textMesh.draw();
        unbindFontTexture();
    }

    if(!minimized){
        for(int i = 0; i < (int)collection.size(); i++){
            collection[i]->draw();
        }
    }

    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}

bool ofxGuiGroupExtended::setValue(float mx, float my, bool bCheck){

    if( !isGuiDrawing() ){
        bGuiActive = false;
        return false;
    }


    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;

            if(_bUseHeader) {
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
    }

    return false;
}

void ofxGuiGroupExtended::minimize(){
    minimized=true;
    setContentHeight(0);
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiGroupExtended::maximize(){
    minimized=false;
    if(_bVertical) {
        float h = 0;
        for(int i=0;i<(int)collection.size();i++){
            h += collection[i]->getHeight() + spacing;
        }
        setContentHeight(h);
    }
    else {
        float max_h = 0;
        for(int i=0;i<(int)collection.size();i++){
            if(max_h < collection[i]->getHeight()) {
                max_h = collection[i]->getHeight();
            }
        }
        setContentHeight(max_h);
    }
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiGroupExtended::minimizeAll(){
    for(int i=0;i<(int)collection.size();i++){
        ofxGuiGroupExtended * group = dynamic_cast<ofxGuiGroupExtended*>(collection[i]);
        if(group)group->minimize();
    }
}

void ofxGuiGroupExtended::maximizeAll(){
    for(int i=0;i<(int)collection.size();i++){
        ofxGuiGroupExtended * group = dynamic_cast<ofxGuiGroupExtended*>(collection[i]);
        if(group)group->maximize();
    }
}

void ofxGuiGroupExtended::sizeChangedCB(){
    float x=b.x + b.width - getContentWidth();
    float y=b.y + b.height - getContentHeight();
    float _x = x;
    float _y = y;

    if(_bVertical) {
        for(int i=0;i<(int)collection.size();i++){
            collection[i]->setPosition(collection[i]->getPosition().x,y + spacing);
            y += collection[i]->getHeight()+spacing;
        }
        setContentHeight(y - _y);
    }
    else {
        float max_h = 0;
        for(int i=0;i<(int)collection.size();i++){

            collection[i]->setPosition(x,y + spacing);

            x += collection[i]->getWidth() + spacing;
            if(max_h < collection[i]->getHeight()) {
                max_h = collection[i]->getHeight();
            }
        }
        y += max_h+spacing;
        setContentWidth(x - _x);
        setContentHeight(y - _y);
    }

    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiGroupExtended::setAlignHorizontal() {
    _bVertical = false;
}

void ofxGuiGroupExtended::setAlignVertical() {
    _bVertical = true;
}

bool ofxGuiGroupExtended::isAlignedVertical() {
    return _bVertical;
}

void ofxGuiGroupExtended::setShowHeader(bool show) {
    if(show == false) {
        if(minimized)
            maximize();
    }
    _bUseHeader = show;
    sizeChangedCB();
    setNeedsRedraw();

}

void ofxGuiGroupExtended::allowMultipleActiveToggles(bool allow) {
    _bAllowMultiple = allow;
    if(!_bAllowMultiple) {
        setOneToggleActive();
    }
}

bool ofxGuiGroupExtended::setActiveToggle(ofxToggle* toggle) {
    if(!toggle->getParameter().cast<bool>().get()) {
        (ofxToggle)*toggle = true;
        deactivateAllOtherToggles(toggle);
        return true;
    }
    return false;
}

bool ofxGuiGroupExtended::setActiveToggle(int index) {
    if(ofxToggle* toggle = dynamic_cast<ofxToggle*>(collection[index])) {
        return setActiveToggle(toggle);
    }
    else {
        ofLogError("ofxGuiGroupExtended", "cannot activate control " + ofToString(index) + " because it's no ofxMinimalToggle.");
        return false;
    }
}

void ofxGuiGroupExtended::deactivateAllOtherToggles(ofxToggle *toggle) {
    if(!_bAllowMultiple) {
        for(int j = 0; j < (int)collection.size(); j++){
            if(ofxToggle* t = dynamic_cast<ofxToggle*>(collection[j])) {
                if(t != toggle) {
                   (ofxToggle)*t = false;
                }
                else {
                    active_toggle_index = j;
                }
            }
        }
    }
}

void ofxGuiGroupExtended::setOneToggleActive() {
    if(active_toggle_index == -1) {
        for(int i = 0; i < (int)collection.size(); i++){
            if(dynamic_cast<ofxToggle*>(collection[i])) {
                setActiveToggle(i);
                return;
            }
        }
    }
}

int ofxGuiGroupExtended::getActiveToggleIndex() {
    return active_toggle_index;
}

float ofxGuiGroupExtended::getContentHeight() {
    float h = b.height - spacing;
    if(_bUseHeader) {
       h -= header;
    }
    if(parent) {
        h -= spacingNextElement;
    }
    return h;
}

void ofxGuiGroupExtended::setContentHeight(float h) {
    if(_bUseHeader) {
        h += header;
    }
    if(parent) {
        h += spacingNextElement;
    }
    h += spacing;
    b.height = h;
}

float ofxGuiGroupExtended::getContentWidth() {
    float w = b.width - spacing;
    if(parent) {
        w -= spacingNextElement;
    }
    return w;
}

void ofxGuiGroupExtended::setContentWidth(float w) {
    if(parent) {
        w += spacingNextElement;
    }
    w += spacing;
    b.width = w;
}
