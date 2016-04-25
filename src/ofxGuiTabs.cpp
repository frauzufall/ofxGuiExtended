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

	setTheme();

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
									   {"align-items", "stretch"},
									   {"justify-content", "flex-start"},
//									   {"margin-top", 10},
									   {"show-header", false},
									   {"border-width", 0},
									   {"padding", "0 10"}
								   }));

	tabs->setExclusiveToggles(true);
	tabs->getActiveToggleIndex().addListener(this, &ofxGuiTabs::setActiveTab);

}

void ofxGuiTabs::generateDraw(){

	for(unsigned int i = 0; i < pages.size(); i++){
		if(ofxBaseGui* page = dynamic_cast<ofxBaseGui*>(pages.at(i))){
			ofxBaseGui* tab = tabs->getControl(i);
			if(tab->getName() != page->getName()
					|| tab->getFillColor() != page->getBackgroundColor()){
				tab->setName(page->getName());
				tab->setFillColor(page->getBackgroundColor());
				tab->setBackgroundColor(ofColor(page->getBackgroundColor(), 50));
				tabs->updateLayout();
			}
		}
	}

	ofxGuiGroup::generateDraw();

	border.clear();
	bg.clear();
	if(activePage){
		int topy = activePage->getShape().getTop();
		border.moveTo(0, topy-borderWidth);
		bg.moveTo(borderWidth, topy);
		for(ofxBaseGui* tab : tabs->getControls()){
			border.lineTo(tab->getShape().getLeft()+tabs->getPosition().x-borderWidth, topy - borderWidth);
			border.lineTo(tab->getShape().getTopLeft()+tabs->getPosition() - ofPoint(borderWidth,borderWidth));
			border.lineTo(tab->getShape().getTopRight()+tabs->getPosition() + ofPoint(borderWidth, -borderWidth));
			border.lineTo(tab->getShape().getRight()+tabs->getPosition().x+borderWidth, topy - borderWidth);
			bg.lineTo(tab->getShape().getLeft()+tabs->getPosition().x, topy);
			bg.lineTo(tab->getShape().getTopLeft()+tabs->getPosition());
			bg.lineTo(tab->getShape().getTopRight()+tabs->getPosition());
			bg.lineTo(tab->getShape().getRight()+tabs->getPosition().x, topy);
//			if(tab != activeToggle){
//				border.moveTo(tab->getShape().getLeft(), activePage->getShape().getTop());
//				border.lineTo(tab->getShape().getRight(), activePage->getShape().getTop());
//			}
		}
		border.lineTo(getWidth(), topy - borderWidth);
		border.lineTo(getWidth(), getHeight());
		border.lineTo(0, getHeight());
		border.lineTo(0, topy-borderWidth);
		bg.lineTo(getWidth() - borderWidth, topy);
		bg.lineTo(getWidth() - borderWidth, getHeight() - borderWidth);
		bg.lineTo(borderWidth, getHeight() - borderWidth);
		bg.lineTo(borderWidth, topy);
	}
	border.append(bg);


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
			page->setConfig(ofJson({
								   {"position", "static"},
								   {"flex", "auto"},
								   {"align-self", "stretch"},
								   {"margin", 0},
								   {"show-header", false},
								   {"border-width", 0}
								   }));
		}

		ofJson toggleconfig = {
			{"width", tabWidth.get()},
			{"type", "fullsize"},
			{"margin", "0 20 0 0"},
			{"border-width", 0}
		};
		ofxBaseGui* tab = tabs->add<ofxToggle>(name, toggleconfig);
		tab->setTextAlignment(TextAlignment::CENTERED);

		if(pages.size() == 1){
			int index = 0;
			setActiveTab(index);
		}

		invalidateChildShape();

	}

}

void ofxGuiTabs::setActiveTab(int &index){
	tabs->setActiveToggle(index);
	for(auto &e : tabs->getControls()){
//		e->setConfig(ofJson({{"margin-bottom", "0"}}));
	}
	activeToggle = tabs->getControl(index);
//	activeToggle->setConfig(ofJson({{"margin-bottom", "-2"}}));
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

std::string ofxGuiTabs::getClassType(){
	return "tabs";
}

vector<std::string> ofxGuiTabs::getClassTypes(){
	vector<std::string> types = ofxGuiGroup::getClassTypes();
	types.push_back(getClassType());
	return types;
}
