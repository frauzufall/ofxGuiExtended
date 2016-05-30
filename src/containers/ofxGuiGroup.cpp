#include "ofGraphics.h"

#include "ofxGuiGroup.h"
#include "ofxGuiPanel.h"
#include "ofxGuiSliderGroup.h"
#include "ofxGuiTabs.h"
#include "../DOM/Document.h"
#include "../DOM/Layout.h"
#include "../controls/ofxGuiFpsPlotter.h"
#include "../view/JsonConfigParser.h"

ofxGuiGroupHeader::ofxGuiGroupHeader(const ofJson &config):ofxGuiElement(){
	setTheme();
	_setConfig(config);
	registerMouseEvents();
}

void ofxGuiGroupHeader::generateDraw(){
	ofxGuiElement::generateDraw();

	textMesh.clear();
	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			textMesh.append(getTextMesh(_parent->getName(), textPadding, getHeight()/ 2 + 4));
		}
		if(_parent->getMinimized()){
			textMesh.append(getTextMesh("+", getWidth() - textPadding - 10, getHeight() / 2 + 4));
		}else{
			textMesh.append(getTextMesh("-", getWidth()- textPadding - 10, getHeight() / 2 + 4));
		}
	}
}

ofxGuiGroupHeader::~ofxGuiGroupHeader(){
}

void ofxGuiGroupHeader::render() {
	ofColor c = ofGetStyle().color;

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}

	ofxGuiElement::render();

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){

			ofSetColor(textColor);
			bindFontTexture();
			textMesh.draw();
			unbindFontTexture();

		}
	}

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}

}

bool ofxGuiGroupHeader::mousePressed(ofMouseEventArgs & args){

	if(!isHidden()){
		ofRectangle minButton(getWidth() - textPadding * 2 - 10, 0, textPadding * 2 + 10, getHeight());
		minButton.setPosition(localToScreen(minButton.getPosition()));
		if(minButton.inside(args.x, args.y)){
			ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
			if(_parent){
				_parent->toggleMinimize();
				this->setNeedsRedraw();
				return true;
			}
		}
	}

	return ofxGuiElement::mousePressed(args);

}

float ofxGuiGroupHeader::getMinWidth(){
	std::string text = "";

	ofxGuiGroup* _parent = dynamic_cast<ofxGuiGroup*>(parent());
	if(_parent){
		if(_parent->getShowName()){
			text += _parent->getName();
		}
		text += "+";
	}

	return getTextWidth(text)+4*textPadding;
}

float ofxGuiGroupHeader::getMinHeight(){
	return ofxGuiElement::getTextHeight("test")+5;
}

std::string ofxGuiGroupHeader::getClassType(){
	return "group-header";
}

vector<std::string> ofxGuiGroupHeader::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}


/*
 *
 * ofxGuiGroup
 *
 */

ofxGuiGroup::ofxGuiGroup()
	:ofxGuiContainer(){

	setup();

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName)
	:ofxGuiContainer(){

	setup();
	setName(collectionName);

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName, const ofJson & config)
	:ofxGuiGroup(collectionName){

	_setConfig(config);

}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters, const ofJson & config)
	:ofxGuiGroup(_parameters.getName()){

	addParametersFrom(_parameters);
	_setConfig(config);

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName, const std::string& _filename, float x, float y)
	:ofxGuiContainer(collectionName, _filename, x, y){

	setup();

}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y)
	:ofxGuiContainer(_parameters, _filename, x,y){

	setup();

}


ofxGuiGroup::~ofxGuiGroup(){

	showHeader.removeListener(this, &ofxGuiGroup::onHeaderVisibility);

}

void ofxGuiGroup::setup(){

	minimized.set("minimized", false);
	showHeader.set("show-header", true);

	header = add<ofxGuiGroupHeader>(ofJson({{"margin", 0}}));

	showHeader.addListener(this, &ofxGuiGroup::onHeaderVisibility);

	setTheme();

}

void ofxGuiGroup::_setConfig(const ofJson &config){

	ofxGuiContainer::_setConfig(config);
	JsonConfigParser::parse(config, showHeader);

}

void ofxGuiGroup::minimize(){
	minimized = true;
//	blockLayout(true);
//	widthMaximized = getWidth();
//	heightMaximized = getHeight();

	for(auto& child : getControls()){
		child->setHidden(true);
	}

	invalidateChildShape();

//	blockLayout(false);

//	setLayoutSize(header->getWidth(), header->getHeight());
}

void ofxGuiGroup::maximize(){
	minimized = false;
//	blockLayout(true);

	for(auto& child : getControls()){
		child->setHidden(false);
	}
//	setLayoutSize(widthMaximized, heightMaximized);

//	blockLayout(false);
}

void ofxGuiGroup::minimizeAll(){
	for(auto & e: getControls()){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
		if(group){
			group->minimize();
		}
	}
}

void ofxGuiGroup::maximizeAll(){
	for(auto & e: getControls()){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(e);
		if(group){
			group->maximize();
		}
	}
}

bool ofxGuiGroup::getMinimized(){
	return minimized;
}

void ofxGuiGroup::toggleMinimize(){
	if(minimized){
		maximize();
	}else {
		minimize();
	}
}

void ofxGuiGroup::setShowHeader(bool show) {
	if(show == false){
		if(minimized)
			maximize();
	}
	showHeader = show;
	invalidateChildShape();
 }

std::vector<ofxGuiElement*> ofxGuiGroup::getControls(){
	static_assert(std::is_base_of<DOM::Element, ofxGuiElement>(), "ElementType must be an Element or derived from Element.");

	std::vector<ofxGuiElement*> results;

	for (auto& child : _children){
		ofxGuiElement* pChild = dynamic_cast<ofxGuiElement*>(child.get());

		if (pChild && pChild != header){
			results.push_back(pChild);
		}
	}

	return results;
}

ofxGuiElement* ofxGuiGroup::getHeader(){
	return this->header;
}

void ofxGuiGroup::onHeaderVisibility(bool &showing){
	if(getHeader()){
		getHeader()->setHidden(!showing);
	}
}

void ofxGuiGroup::onHeaderHeight(float &height){
	if(getHeader()){
		getHeader()->setHeight(height);
	}
}

std::string ofxGuiGroup::getClassType(){
	return "group";
}

vector<std::string> ofxGuiGroup::getClassTypes(){
	vector<std::string> types = ofxGuiContainer::getClassTypes();
	types.push_back(getClassType());
	return types;
}
