#include "ofxGuiTabs.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiTabs::ofxGuiTabs() :
	ofxGuiGroup(){

	setup();

}

ofxGuiTabs::ofxGuiTabs(const string &collectionName, const ofJson &config) :
	ofxGuiTabs(){

	setName(collectionName);
	_setConfig(config);

}

ofxGuiTabs::ofxGuiTabs(string collectionName, string filename, float x, float y)
	:ofxGuiTabs(collectionName){

	this->filename = filename;
	setPosition(x,y);

}

ofxGuiTabs::~ofxGuiTabs(){
	if(tabs){
		tabs->getActiveToggleIndex().removeListener(this, &ofxGuiTabs::setActiveTab);
	}
	ofRemoveListener(childAdded, this, &ofxGuiTabs::onChildAdd);
}

void ofxGuiTabs::setup(){

	tabs = nullptr;
	pages.clear();
	activeToggle = nullptr;
	activePage = nullptr;

	setBackgroundColor(ofColor(0,0,0,0));
	setShowHeader(false);
//	setBorderWidth(0);
	tabWidth.set("tab width", 100);
	tabHeight.set("tab height", 50);
	setConfig(ofJson({
					  {"flex-direction", "column"}
					 }));

	clear();

	ofAddListener(childAdded, this, &ofxGuiTabs::onChildAdd);

}

void ofxGuiTabs::clear(){

	if(tabs){
		tabs->getActiveToggleIndex().removeListener(this, &ofxGuiTabs::setActiveTab);
	}

	ofxGuiGroup::clear();

	tabs = addGroup("tabs", ofJson({
									   {"height", tabHeight.get()},
									   {"align-items", "flex-end"},
									   {"justify-content", "flex-start"},
									   {"margin", 0}
								   }));
	tabs->setShowHeader(false);
	tabs->setExclusiveToggles(true);
//	tabs->setBorderWidth(0);
//	tabs->setBackgroundColor(ofColor(0,0,0,0));
	tabs->getActiveToggleIndex().addListener(this, &ofxGuiTabs::setActiveTab);

}

void ofxGuiTabs::generateDraw(){
	for(unsigned int i = 0; i < pages.size(); i++){
		if(ofxBaseGui* page = dynamic_cast<ofxBaseGui*>(pages.at(i))){
			tabs->getControl(i)->setName(page->getName());
		}
	}
	ofxGuiGroup::generateDraw();
}

void ofxGuiTabs::onChildAdd(ElementEventArgs &args){

	unsigned int nopages = 2;
	if(this->children().size() < nopages){
		return;
	}

	if(tabs){

		Element* _page = args.element();

		pages.push_back(_page);

		std::string name = "Page " + ofToString(pages.size());
		ofxBaseGui* page = dynamic_cast<ofxBaseGui*>(_page);
		if(page){
			name = page->getName();
			page->setHidden(true);
//			page->setMargin(0);
			page->setConfig(ofJson({
								   {"position", "static"},
								   {"flex", "auto"},
								   {"align-self", "stretch"}
								   }));
		}

		ofJson toggleconfig = {
			{"width", tabWidth.get()},
			{"type", "fullsize"},
			{"margin", 0}
		};
		ofxBaseGui* tab = tabs->add<ofxToggle>(name, toggleconfig);
		tab->setTextAlignment(TextAlignment::CENTERED);
		tab->setFillColor(page->getBackgroundColor());
		tab->setBackgroundColor(ofColor(page->getBackgroundColor(), 50));

		if(pages.size() == 1){
			int index = 0;
			setActiveTab(index);
		}

		invalidateChildShape();

	}

}

void ofxGuiTabs::setActiveTab(int &index){
	tabs->setActiveToggle(index);
	activeToggle = tabs->getControl(index);
	for(auto &e : pages){
		e->setHidden(true);
	}
	pages.at(index)->setHidden(false);
	activePage = pages.at(index);
	invalidateChildShape();
}

ofParameter<int>& ofxGuiTabs::getActiveTabIndex(){
	return tabs->getActiveToggleIndex();
}

Element *ofxGuiTabs::getActiveTab(){
	return activePage;
}

void ofxGuiTabs::setTabHeight(int h){
	tabHeight = h;
	tabs->setHeight(h);
}

void ofxGuiTabs::setTabWidth(int w){
	tabWidth = w;
	for(auto & e: tabs->children()){
		e->setWidth(w);
	}
}

void ofxGuiTabs::maximize(){
	ofxGuiGroup::maximize();
	for(auto &e : pages){
		e->setHidden(true);
	}
	activePage->setHidden(false);
	invalidateChildShape();
}

void ofxGuiTabs::maximizeAll(){
	maximize();
}
