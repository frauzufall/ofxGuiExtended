/*
 * ofPanel.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "ofxGuiPanel.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiPanelHeader::ofxGuiPanelHeader():ofxGuiGroupHeader(){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	registerMouseEvents();
	this->setDraggable(true);
	setTheme();
}

ofxGuiPanelHeader::ofxGuiPanelHeader(const ofJson& config):ofxGuiPanelHeader(){
	_setConfig(config);
}

void ofxGuiPanelHeader::loadIcons(){
	unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
	unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
	loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	loadStencilFromHex(loadIcon, loadIconData);
	loadStencilFromHex(saveIcon, saveIconData);

	loadIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	saveIcon.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
}

void ofxGuiPanelHeader::generateDraw(){
	ofxGuiElement::generateDraw();

	float iconHeight = getHeight()*.5;
	float iconWidth = loadIcon.getWidth()/loadIcon.getHeight()*iconHeight;
	int iconSpacing = iconWidth*.5;

	loadBox.x = getWidth() - (iconWidth * 2 + iconSpacing + textPadding);
	loadBox.y = getHeight() / 2. - iconHeight / 2.;
	loadBox.width = iconWidth;
	loadBox.height = iconHeight;
	saveBox.set(loadBox);
	saveBox.x += iconWidth + iconSpacing;

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			textMesh = getTextMesh(_parent->getName(), textPadding, getHeight() / 2 + 4);
		}
	}
}

void ofxGuiPanelHeader::render() {

	ofColor c = ofGetStyle().color;

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}

	ofxGuiElement::render();

	ofSetColor(textColor);

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){

			ofSetColor(textColor);
			bindFontTexture();
			textMesh.draw();
			unbindFontTexture();


		}
	}

	bool texHackEnabled = ofIsTextureEdgeHackEnabled();
	ofDisableTextureEdgeHack();
	loadIcon.draw(loadBox);
	saveIcon.draw(saveBox);
	if(texHackEnabled){
		ofEnableTextureEdgeHack();
	}

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

bool ofxGuiPanelHeader::mousePressed(ofMouseEventArgs & args){

	if(!isHidden()){
		ofPoint pos = screenToLocal(ofPoint(args.x, args.y));
		if(loadBox.inside(pos)){
			ofNotifyEvent(loadPressedE,this);
			return true;
		}
		if(saveBox.inside(pos)) {
			ofNotifyEvent(savePressedE,this);
			return true;
		}

	}

	return ofxGuiElement::mousePressed(args);

}

float ofxGuiPanelHeader::getMinWidth(){
	generateDraw();
	std::string text = "";

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			text += _parent->getName();
		}
	}

	return getTextWidth(text)+loadBox.width+saveBox.width+4*textPadding;
}

std::string ofxGuiPanelHeader::getClassType(){
	return "panel-header";
}

vector<std::string> ofxGuiPanelHeader::getClassTypes(){
	vector<std::string> types = ofxGuiGroupHeader::getClassTypes();
	types.push_back(getClassType());
	return types;
}


/*
 * Panel
 */

ofxGuiPanel::ofxGuiPanel(const string &collectionName)
	:ofxGuiGroup(){

	setup();
	setName(collectionName);

}

ofxGuiPanel::ofxGuiPanel(const string &collectionName, const ofJson & config)
	:ofxGuiPanel(collectionName){

	_setConfig(config);

}

ofxGuiPanel::ofxGuiPanel(const ofParameterGroup & parameters, const ofJson & config)
:ofxGuiPanel(parameters.getName()){

	addParametersFrom(parameters);
	_setConfig(config);

}

ofxGuiPanel::ofxGuiPanel(const std::string& collectionName, const std::string& filename, float x, float y)
	:ofxGuiPanel(collectionName){

	// TODO set filename
	setPosition(x,y);

}

ofxGuiPanel::ofxGuiPanel(const ofParameterGroup & parameters, const std::string& filename, float x, float y)
	:ofxGuiPanel(parameters.getName()){

	addParametersFrom(parameters);
//	config.filename = filename;
	// TODO set filename
	setPosition(x,y);

}

ofxGuiPanel::~ofxGuiPanel(){

	ofRemoveListener(header->move, this, &ofxGuiPanel::onHeaderMove);
	ofRemoveListener(((ofxGuiPanelHeader*)header)->loadPressedE, this, &ofxGuiPanel::onLoadPressed);
	ofRemoveListener(((ofxGuiPanelHeader*)header)->savePressedE, this, &ofxGuiPanel::onSavePressed);

}

void ofxGuiPanel::setup(){

	if(header){
		removeChild(header);
	}
	header = add<ofxGuiPanelHeader>(ofJson({{"margin", 0}}));
	getHeader()->setHidden(!showHeader.get());

	setTheme();

	ofAddListener(header->move, this, &ofxGuiPanel::onHeaderMove);
	ofAddListener(((ofxGuiPanelHeader*)header)->loadPressedE, this, &ofxGuiPanel::onLoadPressed);
	ofAddListener(((ofxGuiPanelHeader*)header)->savePressedE, this, &ofxGuiPanel::onSavePressed);

}

void ofxGuiPanel::onHeaderMove(DOM::MoveEventArgs &args){
	ofPoint screenHeaderPos = localToScreen(args.position());
	ofPoint screenThisPos = getScreenPosition();
	ofPoint diff = screenHeaderPos-screenThisPos;
	setPosition(getPosition()+diff);
	invalidateChildShape();
}

void ofxGuiPanel::onLoadPressed(){
	loadFromFile(filename);
}

void ofxGuiPanel::onSavePressed(){
	saveToFile(filename);
}

std::string ofxGuiPanel::getClassType(){
	return "panel";
}

vector<std::string> ofxGuiPanel::getClassTypes(){
	vector<std::string> types = ofxGuiGroup::getClassTypes();
	types.push_back(getClassType());
	return types;
}
