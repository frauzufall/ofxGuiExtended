#include "ofxGuiElement.h"
#include "ofxGuiGroup.h"
#include "ofImage.h"
#include "ofBitmapFont.h"
#include "ofxGuiDefaultConfig.h"
#ifndef TARGET_EMSCRIPTEN
#include "ofXml.h"
#endif

#include "JsonConfigParser.h"

using namespace std;


//copied from PR https://github.com/openframeworks/openFrameworks/pull/4988
ofJson loadJson(const std::string & filename){
	ofJson json;
	ofFile jsonFile(filename);
	if(jsonFile.exists()){
		try{
			jsonFile >> json;
		}catch(std::exception & e){
			ofLogError("ofLoadJson") << "error loading json from " + filename + ": " + e.what();
		}catch(...){
			ofLogError("ofLoadJson") << "error loading json from " + filename;
		}
	}else{
		ofLogError("ofLoadJson") << "error loading json from " + filename + ": file doesn't exist";
	}
	return json;
}

bool saveJson(const std::string & filename, ofJson & json){
	ofFile jsonFile(filename, ofFile::WriteOnly);
	try{
		jsonFile << json;
	}catch(std::exception & e){
		ofLogError("ofLoadJson") << "error saving json to " + filename + ": " + e.what();
		return false;
	}catch(...){
		ofLogError("ofLoadJson") << "error saving json to " + filename;
		return false;
	}
	return true;
}



void ofxGuiSetFont(const string & fontPath, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, int dpi){
	ofxGuiElement::loadFont(fontPath, fontsize, _bAntiAliased, _bFullCharacterSet, dpi);
}

void ofxGuiSetBitmapFont(){
	ofxGuiElement::setUseTTF(false);
}

void ofxGuiSetHeaderColor(const ofColor & color){
	ofxGuiElement::setDefaultHeaderBackgroundColor(color);
}

void ofxGuiSetBackgroundColor(const ofColor & color){
	ofxGuiElement::setDefaultBackgroundColor(color);
}

void ofxGuiSetBorderColor(const ofColor & color){
	ofxGuiElement::setDefaultBorderColor(color);
}

void ofxGuiSetTextColor(const ofColor & color){
	ofxGuiElement::setDefaultTextColor(color);
}

void ofxGuiSetFillColor(const ofColor & color){
	ofxGuiElement::setDefaultFillColor(color);
}

void ofxGuiSetTextPadding(int padding){
	ofxGuiElement::setDefaultTextPadding(padding);
}

void ofxGuiSetDefaultWidth(int width){
	ofxGuiElement::setDefaultWidth(width);

}

void ofxGuiSetDefaultHeight(int height){
	ofxGuiElement::setDefaultHeight(height);
}

void ofxGuiSetDefaultBorderWidth(float width){
	ofxGuiElement::setDefaultBorderWidth(width);
}

void ofxGuiSetDefaultTheme(const ofJson& theme){
	ofxGuiElement::setDefaultTheme(theme);
}

void ofxGuiLoadDefaultTheme(const std::string& filename){
	ofxGuiElement::loadDefaultTheme(filename);
}

ofColor
ofxGuiElement::defaultHeaderBackgroundColor(64),
ofxGuiElement::defaultBackgroundColor(0,0,0,100),
ofxGuiElement::defaultBorderColor(255),
ofxGuiElement::defaultTextColor(255),
ofxGuiElement::defaultFillColor(128);

float ofxGuiElement::defaultBorderWidth = 0;
int ofxGuiElement::textPadding = 4;
int ofxGuiElement::defaultWidth = 200;
int ofxGuiElement::defaultHeight = 25;
float ofxGuiElement::defaultFontSize = 5;

ofJson ofxGuiElement::defaultTheme = ofxGuiDefaultConfig::get();

ofTrueTypeFont ofxGuiElement::font;
bool ofxGuiElement::fontLoaded = false;
bool ofxGuiElement::useTTF = false;
ofBitmapFont ofxGuiElement::bitmapFont;

ofxGuiElement::ofxGuiElement()
	:DOM::Element("",0,0,defaultWidth, defaultHeight){

	setup();

}

ofxGuiElement::ofxGuiElement(const ofJson &config)
	:ofxGuiElement(){

	_setConfig(config);

}

ofxGuiElement::~ofxGuiElement(){

	unregisterMouseEvents();

}

void ofxGuiElement::setup(){

#ifndef TARGET_EMSCRIPTEN
	serializer = std::make_shared<ofXml>();
#endif

	bRegisteredForMouseEvents = false;

	headerBackgroundColor.set("header-background-color", defaultHeaderBackgroundColor);
	backgroundColor.set("background-color", defaultBackgroundColor);
	borderColor.set("border-color", defaultBorderColor);
	textColor.set("text-color", defaultTextColor);
	fillColor.set("fill-color", defaultFillColor);
	fontSize.set("font-size", defaultFontSize);
	showName.set("show-name", true);
	borderWidth.set("border-width", defaultBorderWidth);
	setLayoutPosition(DOM::LayoutPosition::STATIC);

	textAlignment.setName("text-align");
	setTextAlignment("left");
	setShowName(showName);

	setTheme(defaultTheme);


	// parameter won't be saved to file
	parameter.setSerializable(false);

	registerMouseEvents();

//	setHeight(defaultHeight);
//	setWidth(defaultWidth);

}

void ofxGuiElement::registerMouseEvents(int priority){
	if(bRegisteredForMouseEvents == true){
		return; // already registered.
	}
	bRegisteredForMouseEvents = true;
	ofRegisterMouseEvents(this, priority);
}

void ofxGuiElement::unregisterMouseEvents(int priority){
	if(bRegisteredForMouseEvents == false){
		return; // not registered.
	}
	ofUnregisterMouseEvents(this, priority);
	bRegisteredForMouseEvents = false;
}

void ofxGuiElement::setConfig(const ofJson &config, bool recursive){
	_setConfig(config);
	if(recursive){
		for(auto& e: children()){
			ofxGuiElement* _e = dynamic_cast<ofxGuiElement*>(e);
			if(_e){
				_e->setConfig(config, recursive);
			}
		}
	}
}

void ofxGuiElement::setTheme(const ofJson &config){
	theme = config;
	_setConfigUsingClassifiers(config, true);
}

void ofxGuiElement::loadConfig(const string &filename, bool recursive){
	setConfig(loadJson(filename), recursive);
}

void ofxGuiElement::loadTheme(const string &filename){
	setTheme(loadJson(filename));
}

void ofxGuiElement::_setConfigUsingClassifiers(const ofJson &config, bool recursive){


	for(std::string classifier : this->getClassTypes()){
		ofJson::const_iterator it = config.find(classifier);
		if(it != config.end()){
			if(classifier == "tabs"){
//				cout << classifier << ": " << *it << endl;
			}
			setConfig(*it, false);
		}
	}

	if(recursive){
		for(auto& e: children()){
			ofxGuiElement* _e = dynamic_cast<ofxGuiElement*>(e);
			if(_e){
				_e->_setConfigUsingClassifiers(config, recursive);
			}
		}
	}

}

void ofxGuiElement::_setConfig(const ofJson &config){

	ofJson _config = config;

	if(!_config.is_null() && _config.size() > 0){

		//parse colors
		JsonConfigParser::parse(_config, backgroundColor);
		JsonConfigParser::parse(_config, borderColor);
		JsonConfigParser::parse(_config, textColor);
		JsonConfigParser::parse(_config, fillColor);
		JsonConfigParser::parse(_config, headerBackgroundColor);

		JsonConfigParser::parse(_config, showName);
		JsonConfigParser::parse(_config, fontSize);
		JsonConfigParser::parse(_config, borderWidth);

		//parse size
		JsonConfigParser::parse(_config, this);

		//parse position type
		DOM::LayoutPosition _position = getAttribute<DOM::LayoutPosition>("position");
		JsonConfigParser::parse(_config, "position", _position);
		if(_position != getLayoutPosition()){
			setLayoutPosition(_position);
			invalidateChildShape();
		}

		//parse margin
		if(_config.find("margin") != _config.end()){
			std::string val = ofToString(_config["margin"]);
			vector<std::string> margins = ofSplitString(val, " ");
			std::string val_top, val_right, val_bottom, val_left;
			if(margins.size() == 1){
				val_top = val;
				val_right = val;
				val_bottom = val;
				val_left = val;
			}
			if(margins.size() == 2){
				val_top = margins[0];
				val_right = margins[1];
				val_bottom = margins[0];
				val_left = margins[1];
			}
			if(margins.size() == 3){
				val_top = margins[0];
				val_right = margins[1];
				val_bottom = margins[2];
				val_left = margins[1];
			}
			if(margins.size() == 4){
				val_top = margins[0];
				val_right = margins[1];
				val_bottom = margins[2];
				val_left = margins[3];
			}
			if(_config.find("margin-top") == _config.end()){
				_config["margin-top"] = val_top;
			}
			if(_config.find("margin-bottom") == _config.end()){
				_config["margin-bottom"] = val_bottom;
			}
			if(_config.find("margin-left") == _config.end()){
				_config["margin-left"] = val_left;
			}
			if(_config.find("margin-right") == _config.end()){
				_config["margin-right"] = val_right;
			}
		}

		//parse padding
		if(_config.find("padding") != _config.end()){
			std::string val = ofToString(_config["padding"]);
			vector<std::string> paddings = ofSplitString(val, " ");
			std::string val_top, val_right, val_bottom, val_left;
			if(paddings.size() == 1){
				val_top = val;
				val_right = val;
				val_bottom = val;
				val_left = val;
			}
			if(paddings.size() == 2){
				val_top = paddings[0];
				val_right = paddings[1];
				val_bottom = paddings[0];
				val_left = paddings[1];
			}
			if(paddings.size() == 3){
				val_top = paddings[0];
				val_right = paddings[1];
				val_bottom = paddings[2];
				val_left = paddings[1];
			}
			if(paddings.size() == 4){
				val_top = paddings[0];
				val_right = paddings[1];
				val_bottom = paddings[2];
				val_left = paddings[3];
			}
			if(_config.find("padding-top") == _config.end()){
				_config["padding-top"] = val_top;
			}
			if(_config.find("padding-bottom") == _config.end()){
				_config["padding-bottom"] = val_bottom;
			}
			if(_config.find("padding-left") == _config.end()){
				_config["padding-left"] = val_left;
			}
			if(_config.find("padding-right") == _config.end()){
				_config["padding-right"] = val_right;
			}
		}

		//parse text alignment
		if (_config.find(textAlignment.getName()) != _config.end()) {
			std::string val = _config[textAlignment.getName()];
			setTextAlignment(val);
		}

		//parse all config entries to attribute values of the element.
		//WARNING this will crash if there are non string keys in the config
		for (ofJson::const_iterator it = _config.begin(); it != _config.end(); ++it) {
			std::string key = "_" + it.key();
			if(it.value().is_string() || it.value().is_number() || it.value().is_boolean()){
				std::string value;
				if(it.value().is_string()){
					value = it.value();
				}else {
					value = ofToString(it.value());
				}
				setAttribute(key, value);
			}
		}

		invalidateChildShape();

	}

}

ofAbstractParameter& ofxGuiElement::getParameter(){
	return parameter;
}

void ofxGuiElement::loadFont(const std::string& filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, int dpi){
	font.load(filename, fontsize, _bAntiAliased, _bFullCharacterSet, false, 0, dpi);
	fontLoaded = true;
	useTTF = true;
}

void ofxGuiElement::setUseTTF(bool bUseTTF){
	if(bUseTTF && !fontLoaded){
		loadFont(OF_TTF_MONO, defaultFontSize, true, true);
	}
	useTTF = bUseTTF;
}

void ofxGuiElement::bindFontTexture(){
	if(useTTF){
		font.getFontTexture().bind();
	}else{
		bitmapFont.getTexture().bind();
	}
}

void ofxGuiElement::unbindFontTexture(){
	if(useTTF){
		font.getFontTexture().unbind();
	}else{
		bitmapFont.getTexture().unbind();
	}
}

ofMesh ofxGuiElement::getTextMesh(const string & text, ofPoint p){
	return getTextMesh(text, p.x, p.y);
}

ofMesh ofxGuiElement::getTextMesh(const string & text, float x, float y){
	if(useTTF){
		return font.getStringMesh(text, x, y);
	}else{
		return bitmapFont.getMesh(text, x, y);
	}
}

ofRectangle ofxGuiElement::getTextBoundingBox(const string & text, float x, float y){
	if(useTTF){
		return font.getStringBoundingBox(text, x, y);
	}else{
		return bitmapFont.getBoundingBox(text, x, y);
	}
}

void ofxGuiElement::saveToFile(const std::string& filename){
	if(serializer){
		serializer->load(filename);
		saveTo(*serializer);
		serializer->save(filename);
	}else{
		ofLogError("ofxGui") << "element has no serializer to save to";
	}
}

void ofxGuiElement::loadFromFile(const std::string& filename){
	if(serializer){
		serializer->load(filename);
		loadFrom(*serializer);
	}else{
		ofLogError("ofxGui") << "element has no serializer to load from";
	}
}


void ofxGuiElement::saveTo(ofBaseSerializer & serializer){
	serializer.serialize(getParameter());
}

void ofxGuiElement::loadFrom(ofBaseSerializer & serializer){
	serializer.deserialize(getParameter());
}


void ofxGuiElement::setDefaultSerializer(std::shared_ptr <ofBaseFileSerializer> _serializer){
	serializer = _serializer;
}

string ofxGuiElement::getName(){
	return getParameter().getName();
}

void ofxGuiElement::setName(const std::string& _name){
	getParameter().setName(_name);
}

void ofxGuiElement::setTextAlignment(const TextAlignment &textLayout){
	this->textAlignment = textLayout;
}

void ofxGuiElement::setTextAlignment(const std::string &textLayout){
	if(textLayout == "left"){
		setTextAlignment(TextAlignment::LEFT);
	}
	else if(textLayout == "right"){
		setTextAlignment(TextAlignment::RIGHT);
	}
	else if(textLayout == "center"){
		setTextAlignment(TextAlignment::CENTERED);
	}
}

TextAlignment ofxGuiElement::getTextAlignment() const {
	return textAlignment;
}

ofColor ofxGuiElement::getHeaderBackgroundColor() const {
	return headerBackgroundColor;
}

ofColor ofxGuiElement::getBackgroundColor() const {
	return backgroundColor;
}

ofColor ofxGuiElement::getBorderColor() const {
	return borderColor;
}

ofColor ofxGuiElement::getTextColor() const {
	return textColor;
}

ofColor ofxGuiElement::getFillColor() const {
	return fillColor;
}

bool ofxGuiElement::getShowName() const {
	return showName;
}

float ofxGuiElement::getBorderWidth() const {
	return borderWidth;
}

void ofxGuiElement::setHeaderBackgroundColor(const ofColor & color){
	setNeedsRedraw();
	headerBackgroundColor = color;
}

void ofxGuiElement::setBackgroundColor(const ofColor & color){
	setNeedsRedraw();
	backgroundColor = color;
}

void ofxGuiElement::setBorderColor(const ofColor & color){
	setNeedsRedraw();
	borderColor = color;
}

void ofxGuiElement::setTextColor(const ofColor & color){
	setNeedsRedraw();
	textColor = color;
}

void ofxGuiElement::setFillColor(const ofColor & color){
	setNeedsRedraw();
	fillColor = color;
}

void ofxGuiElement::setBorderWidth(float width){
	setNeedsRedraw();
	borderWidth = width;
}

void ofxGuiElement::setFontSize(float size){
	fontSize = size;
	invalidateChildShape();
}

void ofxGuiElement::setDefaultHeaderBackgroundColor(const ofColor & color){
	defaultHeaderBackgroundColor = color;
}

void ofxGuiElement::setDefaultBackgroundColor(const ofColor & color){
	defaultBackgroundColor = color;
}

void ofxGuiElement::setDefaultBorderColor(const ofColor & color){
	defaultBorderColor = color;
}

void ofxGuiElement::setDefaultTextColor(const ofColor & color){
	defaultTextColor = color;
}

void ofxGuiElement::setDefaultFillColor(const ofColor & color){
	defaultFillColor = color;
}

void ofxGuiElement::setDefaultTextPadding(int padding){
	textPadding = padding;
}

void ofxGuiElement::setDefaultWidth(int width){
	defaultWidth = width;
}

void ofxGuiElement::setDefaultHeight(int height){
	defaultHeight = height;
}

void ofxGuiElement::setDefaultBorderWidth(float width){
	defaultBorderWidth = width;
}

void ofxGuiElement::setDefaultFontSize(float size){
	defaultFontSize = size;
}

void ofxGuiElement::setDefaultTheme(const ofJson &theme){
	defaultTheme = theme;
}

void ofxGuiElement::loadDefaultTheme(const string &filename){
	defaultTheme = loadJson(filename);
}

void ofxGuiElement::setLayoutPosition(DOM::LayoutPosition type){
	setAttribute("position", type);
}

DOM::LayoutPosition ofxGuiElement::getLayoutPosition() {
	if(!hasAttribute("position")){
		setAttribute("position", DOM::LayoutPosition::STATIC);
	}
	return getAttribute<DOM::LayoutPosition>("position");
}

void ofxGuiElement::setShowName(bool show){
	showName = show;
	setNeedsRedraw();
}

void ofxGuiElement::generateDraw(){

	bg.clear();

	bg.setFillColor(backgroundColor);
	bg.setFilled(true);
	bg.setStrokeWidth(0);
	bg.rectangle((int)borderWidth,(int)borderWidth,(int)(getWidth()-borderWidth*2),(int)(getHeight()-borderWidth*2));

	border.clear();
	border.setFilled(true);
	border.setStrokeWidth(0);
	border.setFillColor(borderColor);
	border.rectangle(0,0,(int)getWidth(),(int)getHeight());
	border.rectangle((int)borderWidth,(int)borderWidth,(int)(getWidth()-borderWidth*2),(int)(getHeight()-borderWidth*2));

}

void ofxGuiElement::render(){

	bg.draw();
	border.draw();

}

string ofxGuiElement::saveStencilToHex(const ofImage & img){
	stringstream strm;
	int width = img.getWidth();
	int height = img.getHeight();
	int n = width * height;
	unsigned char cur = 0;
	int shift = 0;
	strm << "{";
	for(int i = 0; i < n;){
		if(img.getPixels()[i * 4 + 3] > 0){
			cur |= 1 << shift;
		}
		i++;
		if(i % 8 == 0){
			strm << "0x" << hex << (unsigned int)cur;
			cur = 0;
			shift = 0;
			if(i < n){
				strm << ",";
			}
		}else{
			shift++;
		}
	}
	strm << "}";
	return strm.str();
}

void ofxGuiElement::loadStencilFromHex(ofImage & img, unsigned char * data){
	int width = img.getWidth();
	int height = img.getHeight();
	int i = 0;
	ofColor on(255, 255);
	ofColor off(255, 0);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			int shift = i % 8;
			int offset = i / 8;
			bool cur = (data[offset] >> shift) & 1;
			img.setColor(x, y, cur ? on : off);
			i++;
		}
	}
	img.update();
}

float ofxGuiElement::getTextWidth(const std::string & text){
	return getTextBoundingBox(text).width;
//	float _width = 0;
//	ofVboMesh mesh = getTextMesh(text, 0, 0);
//	for(unsigned int i = 0; i < mesh.getVertices().size(); i++){
//		if(mesh.getVertex(i).x > _width){
//			_width = mesh.getVertex(i).x;
//		}
//	}
//	_width += textPadding * 2;
//	return _width;
}

float ofxGuiElement::getTextHeight(const std::string & text){
	return getTextBoundingBox(text).height;
//	float _height = 0;
//	ofVboMesh mesh = getTextMesh(text, 0, 0);
//	for(unsigned int i = 0; i < mesh.getVertices().size(); i++){
//		if(mesh.getVertex(i).y > _height){
//			_height = mesh.getVertex(i).y;
//		}
//	}
//	_height += textPadding * 2;
//	return _height;
}

bool ofxGuiElement::isMouseOver() const{
	return _isMouseOver;
}

void ofxGuiElement::setDraggable(bool draggable){
	_isDraggable = draggable;
}

bool ofxGuiElement::isDraggable() const{
	return _isDraggable;
}

bool ofxGuiElement::isDragging() const{
	return _isDragging;
}

bool ofxGuiElement::mouseDragged(ofMouseEventArgs & args){
	if(!isHidden()){

		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
		}else {
			_isMouseOver = false;
		}

		if(_isDragging){
			setPosition(args - grabPoint - getScreenPosition());
			return true;
		}

	}else {
		_isDragging = false;
		_isMouseOver= false;
	}
	return false;
}

bool ofxGuiElement::mousePressed(ofMouseEventArgs & args){
	if(!isHidden()){
		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
			if(_isDraggable){
				_isDragging = true;
				grabPoint = ofPoint(args.x, args.y) - getScreenPosition();
				return true;
			}
		}else {
			_isDragging = false;
			_isMouseOver = false;
		}
	}else {
		_isDragging = false;
		_isMouseOver= false;
	}
	return false;
}

bool ofxGuiElement::mouseMoved(ofMouseEventArgs & args){
	if(!isHidden()){
		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
		}else {
			_isMouseOver = false;
		}
	}else{
		_isDragging = false;
		_isMouseOver= false;
	}
	return false;
}

bool ofxGuiElement::mouseReleased(ofMouseEventArgs & args){
	if(!isHidden()){
		if(localToScreen(ofRectangle(0,0,getWidth(),getHeight())).inside(args.x, args.y)){
			_isMouseOver = true;
		}else {
			_isMouseOver = false;
		}
	}
	_isDragging = false;
	return false;
}

ofJson ofxGuiElement::getThisConfigTheme(){
	return theme;
}

ofJson ofxGuiElement::getGlobalConfigTheme(){
	return defaultTheme;
}

std::string ofxGuiElement::getClassType(){
	return "base";
}

vector<std::string> ofxGuiElement::getClassTypes(){
	vector<std::string> types;
	types.push_back(getClassType());
	return types;
}
