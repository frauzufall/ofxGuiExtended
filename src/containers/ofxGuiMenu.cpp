#include "../ofxGuiExtended.h"
#include "ofxGuiMenu.h"
#include "ofGraphics.h"
#include "../DOM/Document.h"
#include "../view/ofxDOMLayoutHelper.h"
using namespace std;

//MENU LABEL

ofxGuiMenuLabel::ofxGuiMenuLabel(const std::string& labelName, const std::string & label, const ofJson & config)
	:ofxGuiLabel(){
	this->label.set(labelName,label);
	this->_setConfig(config);
}

void ofxGuiMenuLabel::generateDraw(){
	ofxGuiLabel::generateDraw();
	float lineW = 4;
	float height = this->getHeight();
	float width = height;
	float x = this->getWidth()-width/6*5;
	icon.clear();
	icon.setColor(this->getTextColor());
	icon.setFilled(true);
	icon.rectangle(x,height/6,width-width/3, lineW);
	icon.rectangle(x,height/2-lineW/2,width-width/3, lineW);
	icon.rectangle(x,height/6*5-lineW,width-width/3, lineW);
}

void ofxGuiMenuLabel::render(){
	ofxGuiLabel::render();
	icon.draw();
}

float ofxGuiMenuLabel::getMinWidth(){
	return ofxGuiLabel::getMinWidth()+this->getHeight()/2+10;
}

//SUBMENU LABEL

ofxGuiSubmenuLabel::ofxGuiSubmenuLabel(const std::string& labelName, const std::string & label, const ofJson & config)
	:ofxGuiMenuLabel(labelName, label, config){
}

void ofxGuiSubmenuLabel::generateDraw(){
	ofxGuiLabel::generateDraw();
	float lineW = 2;
	float height = this->getHeight();
	float width = height/2;
	float x = this->getWidth()-width;
	icon.clear();
	icon.setColor(this->getTextColor());
	icon.setFilled(true);
	icon.lineTo(x+lineW, height/6);
	icon.lineTo(x+width/6*5, height/2);
	icon.lineTo(x+lineW, height/6*5);
	icon.lineTo(x, height/6*5);
	icon.lineTo(x+width/6*5-lineW, height/2);
	icon.lineTo(x, height/6);
	icon.close();
}

//MENU

ofxGuiMenu::ofxGuiMenu() :
	ofxGuiContainer(){

	setup();

}

ofxGuiMenu::ofxGuiMenu(const string &collectionName, const ofJson &config) :
	ofxGuiMenu(){

	setName(collectionName);
	_setConfig(config);

}

ofxGuiMenu::ofxGuiMenu(string collectionName, string filename, float x, float y)
	:ofxGuiMenu(collectionName){

	this->filename = filename;
	setPosition(x,y);

}

ofxGuiMenu::ofxGuiMenu(const ofParameterGroup & parameters, const ofJson & config)
:ofxGuiMenu(parameters.getName()){

	addParametersFrom(parameters);
	_setConfig(config);

}

ofxGuiMenu::ofxGuiMenu(const ofParameterGroup & parameters, const std::string& filename, float x, float y)
	:ofxGuiMenu(parameters.getName()){

	addParametersFrom(parameters);
//	config.filename = filename;
	// TODO set filename
	setPosition(x,y);

}

ofxGuiMenu::~ofxGuiMenu(){
	ofRemoveListener(addedTo, this, &ofxGuiMenu::onAdded);
}

void ofxGuiMenu::setup(){

	label = nullptr;
	onMenuLabel = false;
	rootMenu = false;
	changeLabelColor = true;

	setTheme();

	setConfig(ofJson({
		{"flex-direction", "column"},
		{"direction", "vertical"},
		{"position", "absolute"},
		{"type","fullsize"}
	 }));

	clear();

	ofAddListener(addedTo, this, &ofxGuiMenu::onAdded);

	setHidden(true);

}

void ofxGuiMenu::generateDraw(){

	ofxGuiContainer::generateDraw();
	if(label){
		if(rootMenu){
			float paddingTop = ofxDOMLayoutHelper::getPaddingTop(label);
			this->setPosition(label->getShape().getBottomLeft()+ofPoint(-5,7));
		}else{
			float paddingTop = ofxDOMLayoutHelper::getPaddingTop(label);
			this->setPosition(label->getShape().getTopRight()+ofPoint(5, -4));
		}
	}

}

void ofxGuiMenu::onAdded(DOM::ElementEventArgs &args){

	if(ofxGuiContainer* parent = dynamic_cast<ofxGuiContainer*>(args.element())){

		if(rootMenu){
			label = parent->add<ofxGuiMenuLabel>(
						this->getName()+"-menu",
						this->getName(),
						ofJson({{"show-name", false},
								{"background-color", "transparent"},
								{"border-width", 0},
								{"width", 50},
								{"height", 50}}));
		}else{
			label = parent->add<ofxGuiSubmenuLabel>(
						this->getName()+"-menu",
						this->getName(),
						ofJson({{"show-name", false},
								{"border-width", 0},
								{"background-color", "transparent"}}));
		}
		if(label){
			this->setPosition(label->getShape().getTopRight());
			this->generateDraw();
		}
	}
}

bool ofxGuiMenu::mouseMoved(ofMouseEventArgs & args){
	ofxGuiContainer::mouseMoved(args);
	if(label){
		if(label->isMouseOver()){
			if(!onMenuLabel){
				onMenuLabel = true;
				if(isHidden()){
					if(changeLabelColor){
						label->setBackgroundColor(label->getTextColor());
						label->setTextColor(label->getTextColor().getInverted());
					}
					this->setHidden(false);
				}
				hideOtherMenusDown(this, nullptr);
				if(ofxGuiContainer* parent = dynamic_cast<ofxGuiContainer*>(this->parent())){
					hideOtherMenusUp(parent, this);
				}
				return true;
			}
		}else{
			onMenuLabel = false;
		}
	}
	return false;
}

bool ofxGuiMenu::mousePressed(ofMouseEventArgs & args){
	if(label){
		if(!label->isMouseOver()){
			if(!isMouseOver(this)){
				if(!isHidden()){
					this->setHidden(true);
					if(changeLabelColor){
						label->setBackgroundColor(ofColor(0,0,0,0));
						label->setTextColor(label->getTextColor().getInverted());
					}
				}
			}
			hideOtherMenusUpPress(this, this);
		}
	}
	return ofxGuiContainer::mousePressed(args);
}

bool ofxGuiMenu::isMouseOver(ofxGuiElement* el){
	if(el->isMouseOver()){
		return true;
	}else{
		for(DOM::Element* child : el->children()){
			if(ofxGuiElement* _child = dynamic_cast<ofxGuiElement*>(child)){
				if(isMouseOver(_child)){
					return true;
				}
			}
		}
	}
	return false;
}

void ofxGuiMenu::hideOtherMenusUp(DOM::Element* parent, ofxGuiMenu* exception){
	if(parent){
		hideOtherMenusDown(parent, exception);
		DOM::Element* nextparent = parent->parent();
		if(nextparent){
			if(ofxGuiMenu* _parent = dynamic_cast<ofxGuiMenu*>(parent)){
				hideOtherMenusUp(nextparent, _parent);
			}else{
				hideOtherMenusUp(nextparent, exception);
			}
		}
	}
}

void ofxGuiMenu::hideOtherMenusDown(DOM::Element* parent, ofxGuiMenu* exception){
	if(parent){
		for(DOM::Element* child : parent->children()){
			if(ofxGuiMenu* menu = dynamic_cast<ofxGuiMenu*>(child)){
				if(menu != exception){
					if(!menu->isHidden()){
						menu->setHidden(true);
						if(menu->changeLabelColor){
							menu->label->setBackgroundColor(ofColor(0,0,0,0));
							menu->label->setTextColor(menu->label->getTextColor().getInverted());
						}
					}
					hideOtherMenusDown(menu, exception);
				}
			}else{
				hideOtherMenusDown(child, exception);
			}
		}
	}
}

void ofxGuiMenu::hideOtherMenusUpPress(ofxGuiContainer* parent, ofxGuiMenu* exception){
	if(parent){
		hideOtherMenusDownPress(parent, exception);
		if(ofxGuiContainer* nextparent = dynamic_cast<ofxGuiContainer*>(parent->parent())){
			if(ofxGuiMenu* _parent = dynamic_cast<ofxGuiMenu*>(parent)){
				hideOtherMenusUpPress(nextparent, _parent);
			}
		}
	}
}

void ofxGuiMenu::hideOtherMenusDownPress(ofxGuiContainer* parent, ofxGuiMenu* exception){
	if(parent){
		for(ofxGuiElement* child : parent->getControls()){
			if(ofxGuiMenu* menu = dynamic_cast<ofxGuiMenu*>(child)){
				if(menu != exception){
					if(!isMouseOver(menu) && ! menu->getMenuLabel()->isMouseOver()){
						if(!menu->isHidden()){
							menu->setHidden(true);
							if(menu->changeLabelColor){
								menu->label->setBackgroundColor(ofColor(0,0,0,0));
								menu->label->setTextColor(menu->label->getTextColor().getInverted());
							}
						}
					}
					hideOtherMenusDownPress(menu, exception);
				}
			}
		}
	}
}

ofxGuiLabel* ofxGuiMenu::getMenuLabel(){
	return label;
}

std::string ofxGuiMenu::getClassType(){
	return "menu";
}

vector<std::string> ofxGuiMenu::getClassTypes(){
	vector<std::string> types = ofxGuiContainer::getClassTypes();
	types.push_back(getClassType());
	return types;
}

// ROOT MENU

ofxGuiRootMenu::ofxGuiRootMenu(const std::string &collectionName, const ofJson & config)
	:ofxGuiMenu(collectionName, config){
	rootMenu = true;
}

ofxGuiRootMenu::~ofxGuiRootMenu(){

}

// COLOR MENU

template<class ColorType>
ofxGuiMenuColor_<ColorType>::ofxGuiMenuColor_(ofParameter<ofColor_<ColorType>> &value, const ofJson &config)
	:ofxGuiMenu(){

	changeLabelColor = false;

	setName(value.getName());

	names.clear();
	names.push_back("r");
	names.push_back("g");
	names.push_back("b");
	names.push_back("a");

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxGuiMenuColor_::changeValue);

	ofColor_<ColorType> val = value;
	ofColor_<ColorType> min = value.getMin();
	ofColor_<ColorType> max = value.getMax();

	for (int i=0; i<4; i++) {
		ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
		add<ofxGuiSlider<ColorType>>(p);
		p.addListener(this, & ofxGuiMenuColor_::changeSlider);
		getControl(names[i])->setConfig(ofJson({
			{"fill-color", ofxGui::colorToString(value.get())}
		}));
	}

	sliderChanging = false;

	_setConfig(config);

}


template<class ColorType>
ofxGuiMenuColor_<ColorType>::~ofxGuiMenuColor_(){
	this->value.removeListener(this, & ofxGuiMenuColor_::changeValue);
	for (int i=0; i<4; i++){
		getControl(names[i])->getParameter().template cast<ColorType>().removeListener(this, &ofxGuiMenuColor_::changeSlider);
	}
}

template<class ColorType>
void ofxGuiMenuColor_<ColorType>::changeSlider(const void * parameter, ColorType & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName()) - getControlIndex(names[0]);
	ofColor_<ColorType> data = value;
	data[i] = _value;

	value = data;

	for (int i=0; i<4; i++){
		getControl(names[i])->setFillColor(value.get());
	}
	if(label){
		label->setBackgroundColor(value.get());
	}
	sliderChanging = false;
}

template<class ColorType>
void ofxGuiMenuColor_<ColorType>::changeValue(ofColor_<ColorType> & value){
	if (sliderChanging){
		return;
	}
	for (int i=0; i<4; i++){
		getControl(names[i])->getParameter().template cast<ColorType>() = value[i];
		getControl(names[i])->setFillColor(value);
	}
	if(label){
		label->setBackgroundColor(value);
	}
}

template<class ColorType>
void ofxGuiMenuColor_<ColorType>::generateDraw(){
	if(label){
		label->setBackgroundColor(value.get());
	}
	ofxGuiMenu::generateDraw();
}

template<class ColorType>
ofAbstractParameter & ofxGuiMenuColor_<ColorType>::getParameter(){
	return value;
}

template<class ColorType>
ofColor_<ColorType> ofxGuiMenuColor_<ColorType>::operator=(const ofColor_<ColorType> & v){
	value = v;
	return value;
}

template<class ColorType>
ofxGuiMenuColor_<ColorType>::operator const ofColor_<ColorType> & (){
	return value;
}

template class ofxGuiMenuColor_<unsigned char>;
template class ofxGuiMenuColor_<unsigned short>;
template class ofxGuiMenuColor_<float>;

//VECTOR MENU

template<class VecType>
ofxGuiMenuVec_<VecType>::ofxGuiMenuVec_(ofParameter<VecType> &value, const ofJson & config)
:ofxGuiMenu(){

	sliderChanging = false;

	setName(value.getName());

	names.clear();
	names.push_back("x");
	names.push_back("y");
	names.push_back("z");
	names.push_back("w");

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxGuiMenuVec_::changeValue);

	VecType val = value;
	VecType min = value.getMin();
	VecType max = value.getMax();
	for (int i=0; i<VecType::DIM; i++) {
		ofParameter<float> p(names[i], val[i], min[i], max[i]);
		add(p);
		p.addListener(this, & ofxGuiMenuVec_::changeSlider);
	}

	_setConfig(config);

}

template<class VecType>
ofxGuiMenuVec_<VecType>::~ofxGuiMenuVec_(){
	this->value.removeListener(this, & ofxGuiMenuVec_::changeValue);
	for (int i=0; i<VecType::DIM; i++) {
		getControl(names[i])->getParameter().template cast<float>().removeListener(this, &ofxGuiMenuVec_::changeSlider);
	}
}

template<class VecType>
void ofxGuiMenuVec_<VecType>::changeSlider(const void * parameter, float & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName()) - getControlIndex(names[0]);
	VecType data = value;
	data[i] = _value;
	value = data;
	sliderChanging = false;
}

template<class VecType>
void ofxGuiMenuVec_<VecType>::changeValue(VecType & value){
	if (sliderChanging){
		return;
	}
	for (int i=0; i<VecType::DIM; i++){
		getControl(names[i])->getParameter().template cast<float>() = value[i];
	}
}

template<class VecType>
ofAbstractParameter & ofxGuiMenuVec_<VecType>::getParameter(){
	return value;
}

template<class VecType>
VecType ofxGuiMenuVec_<VecType>::operator=(const VecType & v){
	value = v;
	return value;
}

template<class VecType>
ofxGuiMenuVec_<VecType>::operator const VecType & (){
	return value;
}

template<class VecType>
const VecType * ofxGuiMenuVec_<VecType>::operator->(){
	return &value.get();
}

template class ofxGuiMenuVec_<ofVec2f>;
template class ofxGuiMenuVec_<ofVec3f>;
template class ofxGuiMenuVec_<ofVec4f>;


// RECTANGLE MENU

ofxGuiMenuRectangle::ofxGuiMenuRectangle(ofParameter<ofRectangle> &value, const ofJson & config)
:ofxGuiMenu(){

	sliderChanging = false;

	setName(value.getName());

	names.clear();
	names.push_back("x");
	names.push_back("y");
	names.push_back("width");
	names.push_back("height");

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxGuiMenuRectangle::changeValue);

	ofRectangle val = value;
	ofRectangle min = value.getMin();
	ofRectangle max = value.getMax();
	ofParameter<float> px(names[0], val.x, min.x, max.x);
	ofParameter<float> py(names[1], val.y, min.y, max.y);
	ofParameter<float> pw(names[2], val.width, min.width, max.width);
	ofParameter<float> ph(names[3], val.height, min.height, max.height);
	add(px);
	add(py);
	add(pw);
	add(ph);
	px.addListener(this, & ofxGuiMenuRectangle::changeSlider);
	py.addListener(this, & ofxGuiMenuRectangle::changeSlider);
	pw.addListener(this, & ofxGuiMenuRectangle::changeSlider);
	ph.addListener(this, & ofxGuiMenuRectangle::changeSlider);

	_setConfig(config);

}

ofxGuiMenuRectangle::~ofxGuiMenuRectangle(){
	this->value.removeListener(this, & ofxGuiMenuRectangle::changeValue);
	for (int i=0; i<4; i++){
		getControl(names[i])->getParameter().template cast<float>().removeListener(this, &ofxGuiMenuRectangle::changeSlider);
	}
}

void ofxGuiMenuRectangle::changeSlider(const void * parameter, float & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName()) - getControlIndex(names[0]);
	ofRectangle data = value;
	switch(i){
		case 0: data.x = _value; break;
		case 1: data.y = _value; break;
		case 2: data.width = _value; break;
		case 3: data.height = _value; break;
	}
	value = data;
	sliderChanging = false;
}

void ofxGuiMenuRectangle::changeValue(ofRectangle & value){
	if (sliderChanging){
		return;
	}
	getControl("x")->getParameter().template cast<float>() = value.x;
	getControl("y")->getParameter().template cast<float>() = value.y;
	getControl("width")->getParameter().template cast<float>() = value.width;
	getControl("height")->getParameter().template cast<float>() = value.height;
}

ofAbstractParameter & ofxGuiMenuRectangle::getParameter(){
	return value;
}

ofRectangle ofxGuiMenuRectangle::operator=(const ofRectangle & v){
	value = v;
	return value;
}

ofxGuiMenuRectangle::operator const ofRectangle & (){
	return value;
}

const ofRectangle * ofxGuiMenuRectangle::operator->(){
	return &value.get();
}
