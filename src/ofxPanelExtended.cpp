#include "ofxPanelExtended.h"
#include "ofGraphics.h"
#include "ofImage.h"
using namespace std;

ofImage ofxPanelExtended::loadIcon;
ofImage ofxPanelExtended::saveIcon;

ofxPanelExtended::ofxPanelExtended()
:bGrabbed(false){}

ofxPanelExtended::ofxPanelExtended(const ofParameterGroup & parameters, string filename, float x, float y)
: ofxGuiGroupExtended(parameters, filename, x, y)
, bGrabbed(false){
    if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
        loadIcons();
    }
    registerMouseEvents();
    setNeedsRedraw();
}

ofxPanelExtended::~ofxPanelExtended(){
    unregisterMouseEvents();
}

ofxPanelExtended * ofxPanelExtended::setup(string collectionName, string filename, float x, float y){
    if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
        loadIcons();
    }
    registerMouseEvents();
    return (ofxPanelExtended*)ofxGuiGroup::setup(collectionName,filename,x,y);
}

ofxPanelExtended * ofxPanelExtended::setup(const ofParameterGroup & parameters, string filename, float x, float y){
    if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
        loadIcons();
    }
    registerMouseEvents();
    return (ofxPanelExtended*)ofxGuiGroup::setup(parameters,filename,x,y);
}

void ofxPanelExtended::loadIcons(){
    unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
    unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
    loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
    saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
    loadStencilFromHex(loadIcon, loadIconData);
    loadStencilFromHex(saveIcon, saveIconData);

    loadIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    saveIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
}

void ofxPanelExtended::generateDraw(){
    border.clear();
    border.setFillColor(thisBorderColor);
    border.setFilled(true);
    border.rectangle(b.x,b.y,b.width+1,b.height+1);

    if(_bUseHeader) {
        generateHeader();
    }
}

void ofxPanelExtended::generateHeader() {
    headerBg.clear();
    headerBg.setFillColor(ofColor(thisHeaderBackgroundColor,180));
    headerBg.setFilled(true);
    headerBg.rectangle(b.x,b.y+1,b.width,header);

    float iconHeight = header*.5;
    float iconWidth = loadIcon.getWidth()/loadIcon.getHeight()*iconHeight;
    int iconSpacing = iconWidth*.5;

    loadBox.x = b.getMaxX() - (iconWidth * 2 + iconSpacing + textPadding);
    loadBox.y = b.y + header / 2. - iconHeight / 2.;
    loadBox.width = iconWidth;
    loadBox.height = iconHeight;
    saveBox.set(loadBox);
    saveBox.x += iconWidth + iconSpacing;

    textMesh = getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y);
}

void ofxPanelExtended::render(){
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
        renderHeader();
    }

    for(int i = 0; i < (int)collection.size(); i++){
        collection[i]->draw();
    }

    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}

void ofxPanelExtended::renderHeader() {
    ofSetColor(thisTextColor);

    bindFontTexture();
    textMesh.draw();
    unbindFontTexture();

    bool texHackEnabled = ofIsTextureEdgeHackEnabled();
    ofDisableTextureEdgeHack();
    loadIcon.draw(loadBox);
    saveIcon.draw(saveBox);
    if(texHackEnabled){
        ofEnableTextureEdgeHack();
    }
}

bool ofxPanelExtended::mouseReleased(ofMouseEventArgs & args){
    this->bGrabbed = false;
    if(ofxGuiGroupExtended::mouseReleased(args)) return true;
    if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
        return true;
    }else{
        return false;
    }
}

bool ofxPanelExtended::setValue(float mx, float my, bool bCheck){

    if( !isGuiDrawing() ){
        bGrabbed = false;
        bGuiActive = false;
        return false;
    }
    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;

            if(_bUseHeader) {
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
        }
    } else if( bGrabbed ){
        setPosition(mx - grabPt.x,my - grabPt.y);
        return true;
    }
    return false;
}
