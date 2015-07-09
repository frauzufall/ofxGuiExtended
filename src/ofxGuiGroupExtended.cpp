#include "ofxGuiGroupExtended.h"
using namespace std;

ofxGuiGroupExtended::ofxGuiGroupExtended(){
    vertical = true;
}

ofxGuiGroupExtended::ofxGuiGroupExtended(const ofParameterGroup & parameters, string filename, float x, float y)
    :ofxGuiGroup(parameters, filename, x, y){
    vertical = true;
}

ofxGuiGroupExtended * ofxGuiGroupExtended::setup(string collectionName, string filename, float x, float y){
    ofxGuiGroup::setup(collectionName,filename,x,y);
    return this;
}

ofxGuiGroupExtended * ofxGuiGroupExtended::setup(const ofParameterGroup & _parameters, string _filename, float x, float y){
    ofxGuiGroup::setup(_parameters,filename,x,y);
    return this;
}

void ofxGuiGroupExtended::add(ofxBaseGui * element){
    collection.push_back( element );

    element->setPosition(b.x, b.y + b.height  + spacing);

    b.height += element->getHeight() + spacing;

    //if(b.width<element->getWidth()) b.width = element->getWidth();

    element->unregisterMouseEvents();

    ofxGuiGroupExtended * subgroup = dynamic_cast<ofxGuiGroupExtended*>(element);
    if(subgroup!=NULL){
        subgroup->filename = filename;
        subgroup->parent = this;
        subgroup->setWidthElements(b.width*.98);
    }else{
        if(parent!=NULL){
            element->setSize(b.width*.98,element->getHeight());
            element->setPosition(b.x + b.width-element->getWidth(),element->getPosition().y);
        }
    }

    parameters.add(element->getParameter());
    setNeedsRedraw();
}

void ofxGuiGroupExtended::setHeightElements(float h){
    for(int i=0;i<(int)collection.size();i++){
        collection[i]->setSize(collection[i]->getWidth(), h);
        collection[i]->setPosition(collection[i]->getPosition().y, b.y + b.height-h);
        ofxGuiGroupExtended * subgroup = dynamic_cast<ofxGuiGroupExtended*>(collection[i]);
        if(subgroup!=NULL){
            subgroup->setHeightElements(h*.98);
        }
    }
    sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiGroupExtended::clear(){
    collection.clear();
    parameters.clear();
    b.height = header + spacing + spacingNextElement ;
    sizeChangedCB();
}

bool ofxGuiGroupExtended::mouseMoved(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseMoved(args);
}

bool ofxGuiGroupExtended::mousePressed(ofMouseEventArgs & args){
    return ofxGuiGroup::mousePressed(args);
}

bool ofxGuiGroupExtended::mouseDragged(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseDragged(args);
}

bool ofxGuiGroupExtended::mouseReleased(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseReleased(args);
}

bool ofxGuiGroupExtended::mouseScrolled(ofMouseEventArgs & args){
    return ofxGuiGroup::mouseScrolled(args);
}

void ofxGuiGroupExtended::generateDraw(){
    border.clear();
    border.setFillColor(ofColor(thisBorderColor,180));
    border.setFilled(true);
    border.rectangle(b.x,b.y+ spacingNextElement,b.width+1,b.height);


    headerBg.clear();
    headerBg.setFillColor(thisHeaderBackgroundColor);
    headerBg.setFilled(true);
    headerBg.rectangle(b.x,b.y +1 + spacingNextElement, b.width, header);

    textMesh = getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y+ spacingNextElement);
    if(minimized){
        textMesh.append(getTextMesh("+", b.width-textPadding-8 + b.x, header / 2 + 4+ b.y+ spacingNextElement));
    }else{
        textMesh.append(getTextMesh("-", b.width-textPadding-8 + b.x, header / 2 + 4 + b.y+ spacingNextElement));
    }
}

void ofxGuiGroupExtended::render(){
    ofxGuiGroup::render();
}

void ofxGuiGroupExtended::minimize(){
    minimized=true;
    b.height = header + spacing + spacingNextElement + 1 /*border*/;
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}

void ofxGuiGroupExtended::maximize(){
    minimized=false;
    for(int i=0;i<(int)collection.size();i++){
        b.height += collection[i]->getHeight() + spacing;
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
    float y;
    if(parent){
        y = b.y  + header + spacing + spacingNextElement;
    }else{
        y = b.y  + header + spacing;
    }
    for(int i=0;i<(int)collection.size();i++){
        collection[i]->setPosition(collection[i]->getPosition().x,y + spacing);
        y += collection[i]->getHeight() + spacing;
    }
    b.height = y - b.y;
    if(parent) parent->sizeChangedCB();
    setNeedsRedraw();
}
