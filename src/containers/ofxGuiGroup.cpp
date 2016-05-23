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
	:ofxGuiElement(){

	setup();

}

ofxGuiGroup::ofxGuiGroup(const std::string& collectionName)
	:ofxGuiElement(){

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
	:ofxGuiGroup(collectionName){

	filename = _filename;
	setPosition(x,y);

}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y)
	:ofxGuiGroup(_parameters.getName()){

	filename = _filename;
	setPosition(x,y);
	addParametersFrom(_parameters);

}


ofxGuiGroup::~ofxGuiGroup(){

	showHeader.removeListener(this, &ofxGuiGroup::onHeaderVisibility);
	ofRemoveListener(resize, this, &ofxGuiGroup::onResize);
	ofRemoveListener(childAdded, this, &ofxGuiGroup::onChildAdded);
	ofRemoveListener(addedTo, this, &ofxGuiGroup::onParentAdded);
	unregisterMouseEvents();

}

void ofxGuiGroup::setup(){

	filename.set("filename","settings.xml");

	exclusiveToggles.set("exclusive toggles", false);
	minimized.set("minimized", false);

	showHeader.set("show-header", true);

	header = add<ofxGuiGroupHeader>(ofJson({{"margin", 0}}));

//	createLayout<ofxDOMFlexBoxLayout>(this);

	showHeader.addListener(this, &ofxGuiGroup::onHeaderVisibility);
	ofAddListener(resize, this, &ofxGuiGroup::onResize);
	ofAddListener(childAdded, this, &ofxGuiGroup::onChildAdded);
	ofAddListener(addedTo, this, &ofxGuiGroup::onParentAdded);

	setTheme();

	registerMouseEvents();

}

void ofxGuiGroup::_setConfig(const ofJson &config){

	ofxGuiElement::_setConfig(config);

//	// ugly hack to hide header of horizontally aligned flexbox containers (because the headers don't get aligned well)
//	ofJson _config = config;
//	if(_config.find("flex-direction") != _config.end()){
//		if(_config["flex-direction"] == "row"){
//			if(_config.find("show-header") != _config.end()){
//				_config["show-header"] = false;
//			}
//		}
//	}
//	// ugly hack to hide header of horizontally aligned box containers (because the headers don't get aligned well)
//	if(_config.find("direction") != _config.end()){
//		if(_config["direction"] == "horizontal"){
//			if(_config.find("show-header") != _config.end()){
//				_config["show-header"] = false;
//			}
//		}
//	}

	JsonConfigParser::parse(config, showHeader);

}

void ofxGuiGroup::addParametersFrom(const ofParameterGroup & parameters){
	for(auto & p: parameters){
		if(p->isReadOnly()){
			ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": read only parameters not supported yet in ofxGui";
			continue;
		}
		string type = p->type();
		// TODO is this neccessary?
		if(type == typeid(ofParameter <int32_t> ).name()){
			add(p->cast<int>());
		}else if(type == typeid(ofParameter <uint32_t> ).name()){
			add(p->cast<uint32_t>());
		}else if(type == typeid(ofParameter <int64_t> ).name()){
			add(p->cast<int64_t>());
		}else if(type == typeid(ofParameter <uint64_t> ).name()){
			add(p->cast<uint64_t>());
		}else if(type == typeid(ofParameter <int8_t> ).name()){
			add(p->cast<int8_t>());
		}else if(type == typeid(ofParameter <uint8_t> ).name()){
			add(p->cast<uint8_t>());
		}else if(type == typeid(ofParameter <int16_t> ).name()){
			add(p->cast<int16_t>());
		}else if(type == typeid(ofParameter <uint16_t> ).name()){
			add(p->cast<uint16_t>());
		}else if(type == typeid(ofParameter<float>).name()){
			add(p->cast<float>());
		}else if(type == typeid(ofParameter <double> ).name()){
			add(p->cast<double>());
		}else if(type == typeid(ofParameter<void>).name()){
			add(p->cast<void>());
		}else if(type == typeid(ofParameter<bool>).name()){
			add(p->cast<bool>());
		}else if(type == typeid(ofParameter<ofVec2f>).name()){
			add(p->cast<ofVec2f>());
		}else if(type == typeid(ofParameter<ofVec3f>).name()){
			add(p->cast<ofVec3f>());
		}else if(type == typeid(ofParameter<ofVec4f>).name()){
			add(p->cast<ofVec4f>());
		}else if(type == typeid(ofParameter<ofColor>).name()){
			add(p->cast<ofColor>());
		}else if(type == typeid(ofParameter<ofShortColor>).name()){
			add(p->cast<ofShortColor>());
		}else if(type == typeid(ofParameter<ofFloatColor>).name()){
			add(p->cast<ofFloatColor>());
		}else if(type == typeid(ofParameter<string>).name()){
			add(p->cast<string>());
		}else if(type == typeid(ofParameterGroup).name()){
			//add<ofxGuiGroup>(p->castGroup());
			add<ofxGuiGroup>(static_cast<ofParameterGroup& >(*p));
		}else{
			ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": ofxBaseGroup; no control for parameter of type " << type;

		}
	}
	this->parameters = parameters;
}

ofxGuiButton* ofxGuiGroup::add(ofParameter <void> & parameter, const ofJson & config){
	return add<ofxGuiButton>(parameter, config);
}

ofxGuiToggle* ofxGuiGroup::add(ofParameter <bool> & parameter, const ofJson & config){
	return add<ofxGuiToggle>(parameter, config);
}

ofxGuiLabel *ofxGuiGroup::add(ofParameter <std::string> & parameter, const ofJson & config){
	return add<ofxGuiLabel>(parameter, config);
}

ofxGuiVec2Slider* ofxGuiGroup::add(ofParameter <ofVec2f> & parameter, const ofJson & config){
	return add<ofxGuiVec2Slider>(parameter, config);
}

ofxGuiVec3Slider* ofxGuiGroup::add(ofParameter <ofVec3f> & parameter, const ofJson & config){
	return add<ofxGuiVec3Slider>(parameter, config);
}

ofxGuiVec4Slider* ofxGuiGroup::add(ofParameter <ofVec4f> & parameter, const ofJson & config){
	return add<ofxGuiVec4Slider>(parameter, config);
}

ofxGuiColorSlider* ofxGuiGroup::add(ofParameter <ofColor> & parameter, const ofJson & config){
	return add<ofxGuiColorSlider>(parameter, config);
}

ofxGuiShortColorSlider* ofxGuiGroup::add(ofParameter <ofShortColor> & parameter, const ofJson & config){
	return add<ofxGuiShortColorSlider>(parameter, config);
}

ofxGuiFloatColorSlider* ofxGuiGroup::add(ofParameter <ofFloatColor> & parameter, const ofJson & config){
	return add<ofxGuiFloatColorSlider>(parameter, config);
}

void ofxGuiGroup::add(const ofParameterGroup &parameters){
	addParametersFrom(parameters);
}

ofxGuiLabel* ofxGuiGroup::addLabel(const std::string& label, const ofJson& config){
	return add<ofxGuiLabel>("", label, config);
}

ofxGuiElement* ofxGuiGroup::addSpacer(float width, float height){
	ofxGuiElement* e = add<ofxGuiElement>();
	e->setSize(width, height);
	e->setBorderWidth(0);
	e->setBackgroundColor(ofColor(0,0,0,0));
	return e;
}

ofxGuiElement *ofxGuiGroup::addSpacer(const ofJson& config){
	ofxGuiElement* e = add<ofxGuiElement>();
	e->setConfig(ofJson({
		{"border-width", 0},
		{"background-color", "rgba(0,0,0,0)"}
	}));
	e->setConfig(config);
	return e;
}

ofxGuiFpsPlotter* ofxGuiGroup::addFpsPlotter(const ofJson &config){
	return add<ofxGuiFpsPlotter>(config);
}

ofxGuiGroup* ofxGuiGroup::addGroup(const string &name, const ofJson &config){
	return add<ofxGuiGroup>(name, config);
}

ofxGuiGroup* ofxGuiGroup::addGroup(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxGuiGroup>(parameters, config);
}

ofxGuiPanel* ofxGuiGroup::addPanel(const string &name, const ofJson &config){
	return add<ofxGuiPanel>(name, config);
}

ofxGuiPanel* ofxGuiGroup::addPanel(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxGuiPanel>(parameters, config);
}

ofxGuiTabs* ofxGuiGroup::addTabs(const string &name, const ofJson &config){
	return add<ofxGuiTabs>(name, config);
}

void ofxGuiGroup::clear(){
	parameters.clear();
	while(getControls().size() > 0) {
		if(!removeChild(getControls().at(0))){
			ofLogError("Element::clear") << "Could not remove child";
			return;
		}
	}
	active_toggle_index = -1;
}

//vector <string> ofxGuiGroup::getControlNames() const{
//	vector <string> names;
//	// TODO
////	for(auto & e: collection){
////		names.push_back(e->getName());
////	}
//	return names;
//}

ofxGuiIntSlider* ofxGuiGroup::getIntSlider(const std::string& name){
	return getControlType <ofxGuiIntSlider>(name);
}

ofxGuiFloatSlider* ofxGuiGroup::getFloatSlider(const std::string& name){
	return getControlType <ofxGuiFloatSlider>(name);
}

ofxGuiToggle* ofxGuiGroup::getToggle(const std::string& name){
	return getControlType <ofxGuiToggle>(name);
}

ofxGuiButton* ofxGuiGroup::getButton(const std::string& name){
	return getControlType <ofxGuiButton>(name);
}

ofxGuiGroup* ofxGuiGroup::getGroup(const std::string& name){
	return getControlType <ofxGuiGroup>(name);
}

ofxGuiElement* ofxGuiGroup::getControl(const std::string& name){
	for(auto & e: getControls()){
		if(e){
			if(e->getName() == name){
				return e;
			}
		}
	}
	return nullptr;
}

int ofxGuiGroup::getControlIndex(ofxGuiElement* element){

	for(int i = 0; i < (int)getControls().size(); i++){
		ofxGuiElement *e = getControl(i);
		if(e){
			if(e == element){
				return i;
			}
		}
	}
	return -1;

}

int ofxGuiGroup::getControlIndex(const std::string& name){

	for(int i = 0; i < (int)getControls().size(); i++){
		ofxGuiElement *e = getControl(i);
		if(e){
			if(e->getName() == name){
				return i;
			}
		}
	}
	return -1;
}

void ofxGuiGroup::minimize(){
	minimized = true;
//	blockLayout(true);
//	widthMaximized = getWidth();
//	heightMaximized = getHeight();

	for(auto& child : getControls()){
		child->setHidden(true);
	}

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


std::size_t ofxGuiGroup::getNumControls() {
	return getControls().size();
}

ofxGuiElement * ofxGuiGroup::getControl(std::size_t num){
	if(num < getControls().size()){
		return dynamic_cast <ofxGuiElement *>(getControls().at(num));
	}else{
		return nullptr;
	}
}

bool ofxGuiGroup::getTogglesExclusive() const {
	return exclusiveToggles;
}

void ofxGuiGroup::setExclusiveToggles(bool exclusive) {
	exclusiveToggles = exclusive;
//	if(exclusiveToggles) {
//		setOneToggleActive();
//	}
}

bool ofxGuiGroup::setActiveToggle(ofxGuiToggle* toggle) {
	if(!(*toggle)) {
		*toggle = true;
		deactivateAllOtherToggles(toggle);
		return true;
	}
	return false;
}

bool ofxGuiGroup::setActiveToggle(int index) {
	if(index >= 0 && index < (int)getControls().size()){
		if(ofxGuiToggle* toggle = dynamic_cast<ofxGuiToggle*>(getControls().at(index))) {
			return setActiveToggle(toggle);
		}
		else {
			ofLogError("ofxGuiGroup", "cannot activate control " + ofToString(index) + " because it's no ofxToggle.");
			return false;
		}
	}
	return false;
}

void ofxGuiGroup::deactivateAllOtherToggles(ofxGuiToggle *toggle) {
	if(exclusiveToggles) {
		int active_index = -1;
		for(int i = 0; i < (int)getControls().size(); i++){
			if(ofxGuiToggle* t = dynamic_cast<ofxGuiToggle*>(getControls()[i])) {
				active_index++;
				if(t != toggle) {
				   *t = false;
				}
				else {
					active_toggle_index.set(active_index);
				}
			}
		}
	}
}

void ofxGuiGroup::setOneToggleActive() {
	if(active_toggle_index == -1){
		for(auto &e : getControls()){
			if(ofxGuiToggle* t = dynamic_cast<ofxGuiToggle*>(e)) {
				setActiveToggle(t);
				return;
			}
		}
	}
}


ofParameter<int>& ofxGuiGroup::getActiveToggleIndex() {
	return active_toggle_index;
}

ofAbstractParameter & ofxGuiGroup::getParameter(){
	return parameters;
}

string ofxGuiGroup::getName(){
	return getParameter().getName();
}

void ofxGuiGroup::setName(const std::string& _name){
	getParameter().setName(_name);
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

void ofxGuiGroup::onChildAdded(DOM::ElementEventArgs& args){
	if(ofxGuiElement* e = dynamic_cast<ofxGuiElement*>(args.element())){
		parameters.add(e->getParameter());
		e->setTheme(theme);
	}
	args.element()->invalidateChildShape();
}

void ofxGuiGroup::onParentAdded(DOM::ElementEventArgs& args){
//	parent()->setLayoutSize(0,0, false);
//	this->setLayoutSize(0,0, false);
	copyLayoutFromDocument();
	_parent->invalidateChildShape();
	invalidateChildShape();
}

void ofxGuiGroup::onResize(DOM::ResizeEventArgs & re){

}

std::string ofxGuiGroup::getClassType(){
	return "group";
}

vector<std::string> ofxGuiGroup::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}
