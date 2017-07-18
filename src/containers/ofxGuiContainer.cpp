#include "ofGraphics.h"

#include "ofxGuiContainer.h"
#include "ofxGuiPanel.h"
#include "ofxGuiSliderGroup.h"
#include "ofxGuiTabs.h"
#include "ofxGuiMenu.h"
#include "../DOM/Document.h"
#include "../DOM/Layout.h"
#include "../controls/ofxGuiFpsPlotter.h"
#include "../view/JsonConfigParser.h"


ofxGuiContainer::ofxGuiContainer()
	:ofxGuiElement(){

	setup();

}

ofxGuiContainer::ofxGuiContainer(const std::string& collectionName)
	:ofxGuiElement(){

	setup();
	setName(collectionName);

}

ofxGuiContainer::ofxGuiContainer(const std::string& collectionName, const ofJson & config)
	:ofxGuiContainer(collectionName){

	_setConfig(config);

}

ofxGuiContainer::ofxGuiContainer(const ofParameterGroup & _parameters, const ofJson & config)
	:ofxGuiContainer(_parameters.getName()){

	addParametersFrom(_parameters);
	_setConfig(config);

}

ofxGuiContainer::ofxGuiContainer(const std::string& collectionName, const std::string& _filename, float x, float y)
	:ofxGuiContainer(collectionName){

	filename = _filename;
	setPosition(x,y);

}

ofxGuiContainer::ofxGuiContainer(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y)
	:ofxGuiContainer(_parameters.getName()){

	filename = _filename;
	setPosition(x,y);
	addParametersFrom(_parameters);

}


ofxGuiContainer::~ofxGuiContainer(){

	ofRemoveListener(resize, this, &ofxGuiContainer::onResize);
	ofRemoveListener(childAdded, this, &ofxGuiContainer::onChildAdded);
	ofRemoveListener(addedTo, this, &ofxGuiContainer::onParentAdded);
	unregisterMouseEvents();

}

void ofxGuiContainer::setup(){

	filename.set("filename","settings.xml");

	exclusiveToggles.set("exclusive toggles", false);

	ofAddListener(resize, this, &ofxGuiContainer::onResize);
	ofAddListener(childAdded, this, &ofxGuiContainer::onChildAdded);
	ofAddListener(addedTo, this, &ofxGuiContainer::onParentAdded);

	setTheme();

	registerMouseEvents();

}

void ofxGuiContainer::_setConfig(const ofJson &config){

	ofxGuiElement::_setConfig(config);

}

void ofxGuiContainer::add(const std::shared_ptr<ofAbstractParameter> & p){
	if(p->isReadOnly()){
		ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": read only parameters not supported yet in ofxGui";
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
	}else if(type == typeid(ofParameter<ofRectangle>).name()){
		add(p->cast<ofRectangle>());
	}else if(type == typeid(ofParameter<string>).name()){
		add(p->cast<string>());
	}else if(type == typeid(ofParameterGroup).name()){
		//add<ofxGuiContainer>(p->castGroup());
		add<ofxGuiContainer>(static_cast<ofParameterGroup& >(*p));
	}else{
		ofLogWarning("ofxGui") << "Trying to add " << p->getName() << ": ofxBaseGroup; no control for parameter of type " << type;

	}
}

void ofxGuiContainer::addParametersFrom(const ofParameterGroup & parameters){
	for(auto & p: parameters){
		add(p);
	}
	this->parameters = parameters;
}

ofxGuiButton* ofxGuiContainer::add(ofParameter <void> & parameter, const ofJson & config){
	return add<ofxGuiButton>(parameter, config);
}

ofxGuiToggle* ofxGuiContainer::add(ofParameter <bool> & parameter, const ofJson & config){
	return add<ofxGuiToggle>(parameter, config);
}

ofxGuiLabel *ofxGuiContainer::add(ofParameter <std::string> & parameter, const ofJson & config){
	return add<ofxGuiLabel>(parameter, config);
}

ofxGuiVec2Slider* ofxGuiContainer::add(ofParameter <ofVec2f> & parameter, const ofJson & config){
	return add<ofxGuiVec2Slider>(parameter, config);
}

ofxGuiVec3Slider* ofxGuiContainer::add(ofParameter <ofVec3f> & parameter, const ofJson & config){
	return add<ofxGuiVec3Slider>(parameter, config);
}

ofxGuiVec4Slider* ofxGuiContainer::add(ofParameter <ofVec4f> & parameter, const ofJson & config){
	return add<ofxGuiVec4Slider>(parameter, config);
}

ofxGuiColorSlider* ofxGuiContainer::add(ofParameter <ofColor> & parameter, const ofJson & config){
	return add<ofxGuiColorSlider>(parameter, config);
}

ofxGuiShortColorSlider* ofxGuiContainer::add(ofParameter <ofShortColor> & parameter, const ofJson & config){
	return add<ofxGuiShortColorSlider>(parameter, config);
}

ofxGuiFloatColorSlider* ofxGuiContainer::add(ofParameter <ofFloatColor> & parameter, const ofJson & config){
	return add<ofxGuiFloatColorSlider>(parameter, config);
}

ofxGuiRectangleSlider* ofxGuiContainer::add(ofParameter <ofRectangle> & parameter, const ofJson & config){
	return add<ofxGuiRectangleSlider>(parameter, config);
}

void ofxGuiContainer::add(const ofParameterGroup &parameters){
	addParametersFrom(parameters);
}

ofxGuiLabel* ofxGuiContainer::addLabel(const std::string& label, const ofJson& config){
	return add<ofxGuiLabel>("", label, config);
}

ofxGuiElement* ofxGuiContainer::addSpacer(float width, float height){
	ofxGuiElement* e = add<ofxGuiElement>();
	e->setSize(width, height);
	e->setBorderWidth(0);
	e->setBackgroundColor(ofColor(0,0,0,0));
	return e;
}

ofxGuiElement *ofxGuiContainer::addSpacer(const ofJson& config){
	ofxGuiElement* e = add<ofxGuiElement>();
	e->setConfig(ofJson({
		{"border-width", 0},
		{"background-color", "rgba(0,0,0,0)"}
	}));
	e->setConfig(config);
	return e;
}

ofxGuiFpsPlotter* ofxGuiContainer::addFpsPlotter(const ofJson &config){
	return add<ofxGuiFpsPlotter>(config);
}

ofxGuiGroup* ofxGuiContainer::addGroup(const string &name, const ofJson &config){
	return add<ofxGuiGroup>(name, config);
}

ofxGuiGroup* ofxGuiContainer::addGroup(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxGuiGroup>(parameters, config);
}

ofxGuiContainer* ofxGuiContainer::addContainer(const string &name, const ofJson &config){
	return add<ofxGuiContainer>(name, config);
}

ofxGuiContainer* ofxGuiContainer::addContainer(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxGuiContainer>(parameters, config);
}

ofxGuiPanel* ofxGuiContainer::addPanel(const string &name, const ofJson &config){
	return add<ofxGuiPanel>(name, config);
}

ofxGuiPanel* ofxGuiContainer::addPanel(const ofParameterGroup & parameters, const ofJson &config){
	return add<ofxGuiPanel>(parameters, config);
}

ofxGuiTabs* ofxGuiContainer::addTabs(const string &name, const ofJson &config){
	return add<ofxGuiTabs>(name, config);
}

ofxGuiMenu* ofxGuiContainer::addMenu(ofParameterGroup &content, const ofJson &config){
	ofxGuiMenu* c = add<ofxGuiMenu>(content.getName(), config);
	c->addMenuItems(&content);
	return c;
}

void ofxGuiContainer::addMenuItems(ofParameterGroup *content){
	for(auto& _item : *content){
		addToMenu(_item);
	}
}

void ofxGuiContainer::addToMenu(const std::shared_ptr<ofAbstractParameter> & p){
	string type = p->type();
	if(type == typeid(ofParameter<ofVec2f>).name()){
		add<ofxGuiMenuVec2>(p->cast<ofVec2f>());
	}else if(type == typeid(ofParameter<ofVec3f>).name()){
		add<ofxGuiMenuVec3>(p->cast<ofVec3f>());
	}else if(type == typeid(ofParameter<ofVec4f>).name()){
		add<ofxGuiMenuVec4>(p->cast<ofVec4f>());
	}else if(type == typeid(ofParameter<ofColor>).name()){
		add<ofxGuiMenuColor>(p->cast<ofColor>());
	}else if(type == typeid(ofParameter<ofShortColor>).name()){
		add<ofxGuiMenuShortColor>(p->cast<ofShortColor>());
	}else if(type == typeid(ofParameter<ofFloatColor>).name()){
		add<ofxGuiMenuFloatColor>(p->cast<ofFloatColor>());
	}else if(type == typeid(ofParameter<ofRectangle>).name()){
		add<ofxGuiMenuRectangle>(p->cast<ofRectangle>());
	}else if(type == typeid(ofParameterGroup).name()){
		ofxGuiMenu* c = add<ofxGuiMenu>(p->getName());
		c->addMenuItems(dynamic_cast<ofParameterGroup*>(p.get()));
	}else{
		add(p);
	}
}

void ofxGuiContainer::clear(){
	parameters.clear();
	while(getControls().size() > 0) {
		if(!removeChild(getControls().at(0))){
			ofLogError("Element::clear") << "Could not remove child";
			return;
		}
	}
	active_toggle_index = -1;
}

//vector <string> ofxGuiContainer::getControlNames() const{
//	vector <string> names;
//	// TODO
////	for(auto & e: collection){
////		names.push_back(e->getName());
////	}
//	return names;
//}

ofxGuiIntSlider* ofxGuiContainer::getIntSlider(const std::string& name){
	return getControlType <ofxGuiIntSlider>(name);
}

ofxGuiFloatSlider* ofxGuiContainer::getFloatSlider(const std::string& name){
	return getControlType <ofxGuiFloatSlider>(name);
}

ofxGuiToggle* ofxGuiContainer::getToggle(const std::string& name){
	return getControlType <ofxGuiToggle>(name);
}

ofxGuiButton* ofxGuiContainer::getButton(const std::string& name){
	return getControlType <ofxGuiButton>(name);
}

ofxGuiContainer* ofxGuiContainer::getGroup(const std::string& name){
	return getControlType <ofxGuiContainer>(name);
}

ofxGuiElement* ofxGuiContainer::getControl(const std::string& name){
	for(auto & e: getControls()){
		if(e){
			if(e->getName() == name){
				return e;
			}
		}
	}
	return nullptr;
}

int ofxGuiContainer::getControlIndex(ofxGuiElement* element){

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

int ofxGuiContainer::getControlIndex(const std::string& name){

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

std::vector<ofxGuiElement*> ofxGuiContainer::getControls(){
	static_assert(std::is_base_of<DOM::Element, ofxGuiElement>(), "ElementType must be an Element or derived from Element.");

	std::vector<ofxGuiElement*> results;

	for (auto& child : _children){
		ofxGuiElement* pChild = dynamic_cast<ofxGuiElement*>(child.get());
		results.push_back(pChild);
	}

	return results;
}


std::size_t ofxGuiContainer::getNumControls() {
	return getControls().size();
}

ofxGuiElement * ofxGuiContainer::getControl(std::size_t num){
	if(num < getControls().size()){
		return dynamic_cast <ofxGuiElement *>(getControls().at(num));
	}else{
		return nullptr;
	}
}

bool ofxGuiContainer::getTogglesExclusive() const {
	return exclusiveToggles;
}

void ofxGuiContainer::setExclusiveToggles(bool exclusive) {
	exclusiveToggles = exclusive;
//	if(exclusiveToggles) {
//		setOneToggleActive();
//	}
}

bool ofxGuiContainer::setActiveToggle(ofxGuiToggle* toggle) {
	if(!(*toggle)) {
		*toggle = true;
		deactivateAllOtherToggles(toggle);
		return true;
	}
	return false;
}

bool ofxGuiContainer::setActiveToggle(int index) {
	if(index >= 0 && index < (int)getControls().size()){
		if(ofxGuiToggle* toggle = dynamic_cast<ofxGuiToggle*>(getControls().at(index))) {
			return setActiveToggle(toggle);
		}
		else {
			ofLogError("ofxGuiContainer", "cannot activate control " + ofToString(index) + " because it's no ofxToggle.");
			return false;
		}
	}
	return false;
}

void ofxGuiContainer::deactivateAllOtherToggles(ofxGuiToggle *toggle) {
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

void ofxGuiContainer::setOneToggleActive() {
	if(active_toggle_index == -1){
		for(auto &e : getControls()){
			if(ofxGuiToggle* t = dynamic_cast<ofxGuiToggle*>(e)) {
				setActiveToggle(t);
				return;
			}
		}
	}
}


ofParameter<int>& ofxGuiContainer::getActiveToggleIndex() {
	return active_toggle_index;
}

ofAbstractParameter & ofxGuiContainer::getParameter(){
	return parameters;
}

string ofxGuiContainer::getName(){
	return getParameter().getName();
}

void ofxGuiContainer::setName(const std::string& _name){
	getParameter().setName(_name);
}

void ofxGuiContainer::onChildAdded(DOM::ElementEventArgs& args){
	if(ofxGuiElement* e = dynamic_cast<ofxGuiElement*>(args.element())){
		parameters.add(e->getParameter());
		e->setTheme(theme);
	}
	args.element()->invalidateChildShape();
}

void ofxGuiContainer::onParentAdded(DOM::ElementEventArgs& args){
//	parent()->setLayoutSize(0,0, false);
//	this->setLayoutSize(0,0, false);
	copyLayoutFromDocument();
	_parent->invalidateChildShape();
	invalidateChildShape();
}

void ofxGuiContainer::onResize(DOM::ResizeEventArgs & re){

}

std::string ofxGuiContainer::getClassType(){
	return "container";
}

vector<std::string> ofxGuiContainer::getClassTypes(){
	vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}
