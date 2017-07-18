#include "ofxGuiExtended.h"

ofxGui::ofxGui(){
	setup_done = false;
}

ofxGui::~ofxGui(){
}

void ofxGui::setupFlexBoxLayout(){
	setup<ofxDOMFlexBoxLayout>();
}

DOM::Document* ofxGui::getDocument(){
	if(!setup_done){
		setup();
	}
	return document.get();
}

ofParameter<bool>& ofxGui::getVisible(){
	return getDocument()->getVisible();
}

ofxGuiContainer* ofxGui::addContainer(const std::string& name, const ofJson& config){
	return getDocument()->add<ofxGuiContainer>(name, rootGroupConfig(config));
}

ofxGuiContainer* ofxGui::addContainer(const ofParameterGroup & parameters, const ofJson& config){
	return getDocument()->add<ofxGuiContainer>(parameters, rootGroupConfig(config));
}

ofxGuiGroup* ofxGui::addGroup(const std::string& name, const ofJson& config){
	return getDocument()->add<ofxGuiGroup>(name, rootGroupConfig(config));
}

ofxGuiGroup* ofxGui::addGroup(const ofParameterGroup & parameters, const ofJson& config){
	return getDocument()->add<ofxGuiGroup>(parameters, rootGroupConfig(config));
}

ofxGuiPanel* ofxGui::addPanel(const std::string& name, const ofJson& config){
	return getDocument()->add<ofxGuiPanel>(name, rootGroupConfig(config));
}

ofxGuiPanel* ofxGui::addPanel(const ofParameterGroup & parameters, const ofJson& config){
	return getDocument()->add<ofxGuiPanel>(parameters, rootGroupConfig(config));
}

ofxGuiTabs* ofxGui::addTabs(const std::string& name, const ofJson& config){
	return getDocument()->add<ofxGuiTabs>(name, rootGroupConfig(config));
}

ofJson ofxGui::rootGroupConfig(const ofJson& config){
	ofJson res = config;
	if(config.find("width") == config.end()){
		res["width"] = 200;
	}
	if(config.find("position") == config.end()){
		res["position"] = "absolute";
	}
	if(config.find("left") == config.end()){
		res["left"] = 10;
	}
	if(config.find("top") == config.end()){
		res["top"] = 10;
	}
	if(config.find("flex-direction") == config.end()){
		res["flex-direction"] = "column";
	}
	return res;
}

void ofxGui::setConfig(const ofJson &config){
	for(auto* child : getDocument()->children()){
		if(ofxGuiElement* el = dynamic_cast<ofxGuiElement*>(child)){
			el->setConfig(config, true);
		}
	}
}

ofxGuiContainer* ofxGui::addMenu(ofParameterGroup &content, const ofJson& config){
	ofJson res = config;
	res["width"] = 50;
	ofxGuiContainer* root = addContainer("",res);
	ofxGuiMenu* c = root->add<ofxGuiRootMenu>("", rootGroupConfig(config));
	c->addMenuItems(&content);
	return root;
}

std::string ofxGui::colorToString(const ofColor& color){
	std::stringstream strstr;
	strstr << "rgba(" << (int)color.r << "," << (int)color.g << "," << (int)color.b << "," << ((float)color.a)/255. << ")";
	return strstr.str();
}
