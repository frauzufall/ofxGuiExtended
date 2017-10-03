#include "ofxGuiInputField.h"
#include "ofGraphics.h"

template<typename Type>
ofxGuiInputField<Type>::ofxGuiInputField(){

	setup();

}

template<typename Type>
ofxGuiInputField<Type>::ofxGuiInputField(const ofJson &config)
	:ofxGuiInputField(){

	_setConfig(config);

}

template<typename Type>
ofxGuiInputField<Type>::ofxGuiInputField(ofParameter<Type>& _val, const ofJson &config)
	:ofxGuiInputField(){

	value.makeReferenceTo(_val);
	input = ofToString(value);
	inputWidth = getTextBoundingBox(input,0,0).width;
	value.addListener(this,&ofxGuiInputField::valueChanged);
	pressCounter = 0;
	_setConfig(config);

}

template<typename Type>
ofxGuiInputField<Type>::ofxGuiInputField(const std::string& _name, Type _val, Type _min, Type _max, const ofJson &config)
	:ofxGuiInputField(value.set(_name,_val,_min,_max), config){
}

template<typename Type>
ofxGuiInputField<Type>::~ofxGuiInputField(){
	value.removeListener(this,&ofxGuiInputField::valueChanged);
	unregisterKeyEvents();
}

template<typename Type>
void ofxGuiInputField<Type>::setup(){

	bChangedInternally = false;
	bMousePressed = false;
	mouseInside = false;
	bRegisteredForKeyEvents = false;
	hasFocus = false;
	mousePressedPos = -1;
	selectStartX = -1;
	selectStartPos = -1;
	selectEndPos = -1;
	pressCounter = 0;
	inputWidth = 0;
	selectionWidth = 0;
	setTheme();
	registerMouseEvents();
	registerKeyEvents();

}

template<typename Type>
void ofxGuiInputField<Type>::setMin(Type min){
	value.setMin(min);
}

template<typename Type>
Type ofxGuiInputField<Type>::getMin(){
	return value.getMin();
}

template<typename Type>
void ofxGuiInputField<Type>::setMax(Type max){
	value.setMax(max);
}

template<typename Type>
Type ofxGuiInputField<Type>::getMax(){
	return value.getMax();
}

template<typename Type>
void ofxGuiInputField<Type>::calculateSelectionArea(int selectIdx1, int selectIdx2){
	std::string preSelectStr, selectStr;

	if(selectIdx1 <= selectIdx2){
		selectStartPos = selectIdx1;
		selectEndPos = selectIdx2;
	}else{
		selectStartPos = selectIdx2;
		selectEndPos = selectIdx1;
	}

	float preSelectWidth = 0;
	if(selectStartPos > 0){
		preSelectStr.assign(input,0,selectStartPos);
		preSelectWidth = getTextBoundingBox(preSelectStr,0,0).width;
	}
	selectStartX = getWidth() - textPadding - inputWidth + preSelectWidth;

	if(hasSelectedText()){
		selectStr.assign(input,selectStartPos,selectEndPos-selectStartPos);
		selectionWidth = getTextBoundingBox(selectStr,0,0).width;
	}
}


template<typename Type>
float ofxGuiInputField<Type>::getMinWidth(){
	std::string text = ofToString(value.get());
	if(showName){
		if(!getName().empty()){
			text = getName() + ": ";
		}
	}

	return ofxGuiElement::getTextWidth(text);
}

template<typename Type>
float ofxGuiInputField<Type>::getMinHeight(){
	std::string text = ofToString(value.get());
	if(showName){
		if(!getName().empty()){
			text = getName() + ": ";
		}
	}
	return ofxGuiElement::getTextHeight(text);
}

template<typename Type>
bool ofxGuiInputField<Type>::mouseMoved(ofMouseEventArgs & args){

	ofxGuiElement::mouseMoved(args);

	mouseInside = !isHidden() && isMouseOver();
	return mouseInside;
}

template<typename Type>
bool ofxGuiInputField<Type>::mousePressed(ofMouseEventArgs & args){

	ofxGuiElement::mousePressed(args);

	if(isMouseOver()){
		bMousePressed = true;
		hasFocus = true;

		float cursorX = args.x - (localToScreen(getShape()).getRight() - textPadding - inputWidth);
		int cursorPos = ofMap(cursorX,0,inputWidth,0,input.size(),true);
		mousePressedPos = cursorPos;

		calculateSelectionArea(cursorPos, cursorPos);

		pressCounter++;

	}else{
		if(hasFocus){
			leaveFocus();
		}
	}
	return false;
}

template<typename Type>
bool ofxGuiInputField<Type>::mouseDragged(ofMouseEventArgs & args){

	ofxGuiElement::mouseDragged(args);

	if(!hasFocus || !bMousePressed)
		return false;

	float cursorX = args.x - (localToScreen(getShape()).getRight() - textPadding - inputWidth);
	int cursorPos = ofMap(cursorX,0,inputWidth,0,input.size(),true);
	calculateSelectionArea(mousePressedPos,cursorPos);
	return false;
}

template<typename Type>
bool ofxGuiInputField<Type>::mouseReleased(ofMouseEventArgs & args){

	ofxGuiElement::mouseReleased(args);

	//	if(bUpdateOnEnterOnly){ //TODO not implemented yet
	//		value.enableEvents();
	//	}

	if(hasFocus){
		if(pressCounter == 1 && !hasSelectedText()){
			//activated panel without selecting an area => select all
			calculateSelectionArea(0, input.size());
		}
	}

	bMousePressed = false;
	return false;
}


template<typename Type>
typename std::enable_if<std::is_integral<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return range;
}

template<typename Type>
bool ofxGuiInputField<Type>::mouseScrolled(ofMouseEventArgs & args){

	ofxGuiElement::mouseScrolled(args);

	if(mouseInside || hasFocus){
		if(args.y>0 || args.y<0){
			double range = getRange(value.getMin(),value.getMax(),getWidth());
			Type newValue = value + ofMap(args.y,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			value = newValue;
		}
		return true;
	}else{
		return false;
	}
}

template<>
bool ofxGuiInputField<std::string>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside || hasFocus){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
void ofxGuiInputField<Type>::registerKeyEvents(){
	if(bRegisteredForKeyEvents == true){
		return; // already registered.
	}
	bRegisteredForKeyEvents = true;
	ofRegisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}

template<typename Type>
void ofxGuiInputField<Type>::unregisterKeyEvents(){
	if(bRegisteredForKeyEvents == false){
		return; // not registered.
	}
	ofUnregisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
	bRegisteredForKeyEvents = false;
}

template<typename Type>
bool ofxGuiInputField<Type>::keyPressed(ofKeyEventArgs & args){
	if(hasFocus && !bMousePressed){

		int newCursorIdx = -1;
		if(args.key >= '0' && args.key <= '9'){
			int digit = args.key - '0';
			newCursorIdx = insertKeystroke(ofToString(digit));
		}else if(args.key == '.' ){
			newCursorIdx = insertKeystroke(".");
		}else if(args.key == OF_KEY_BACKSPACE || args.key == OF_KEY_DEL){
			if(hasSelectedText()){
				input.erase(selectStartPos,selectEndPos-selectStartPos);
				newCursorIdx = selectStartPos;
				parseInput();
			}else{
				int deleteIdx = -1;
				if(args.key == OF_KEY_BACKSPACE){
					deleteIdx = selectStartPos-1;
				}else if(args.key == OF_KEY_DEL){
					deleteIdx = selectStartPos;
				}

				//erase char if valid deleteIdx
				if(deleteIdx >= 0 && deleteIdx < (int)input.size()){
					input.erase(deleteIdx,1);
					newCursorIdx = deleteIdx;
					parseInput();
				}
			}
		}else if(args.key == OF_KEY_LEFT){
			if(hasSelectedText()){
				newCursorIdx = selectStartPos;
			}else{
				newCursorIdx = selectStartPos == 0 ? 0 : selectStartPos-1;
			}
		}else if(args.key == OF_KEY_RIGHT){
			if(hasSelectedText()){
				newCursorIdx = selectEndPos;
			}else{
				newCursorIdx = selectStartPos == (int)input.size() ? (int)input.size() : selectStartPos+1;
			}
		}else if(args.key == OF_KEY_RETURN){
			leaveFocus();
		}else if((args.key >= '!' && args.key <= '~')
				 || (args.key <= 'a' && args.key >= 'Z')
				 || (args.key == ' ')){
			newCursorIdx = insertAlphabetic(ofToString((char)args.key));
		}

		if(newCursorIdx != -1){
			//set cursor
			calculateSelectionArea(newCursorIdx,newCursorIdx);
		}
		return true;
	}
	return false;
}

template<typename Type>
bool ofxGuiInputField<Type>::keyReleased(ofKeyEventArgs & args){
	return hasFocus && !bMousePressed;
}


template<typename Type>
int ofxGuiInputField<Type>::insertKeystroke(const std::string & character){
	if(hasSelectedText()){
		input.erase(selectStartPos,selectEndPos-selectStartPos);
	}
	input.insert(selectStartPos,character);
	parseInput();
	return selectStartPos + 1;
}

template<typename Type>
int ofxGuiInputField<Type>::insertAlphabetic(const std::string &){
	return -1; //do nothing for numeric types, cursor/selection area stays the same
}

template<>
int ofxGuiInputField<std::string>::insertAlphabetic(const std::string & character){
	return insertKeystroke(character);
}

template<typename Type>
Type ofxGuiInputField<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxGuiInputField<Type>::operator const Type & (){
	return value;
}

template<typename Type>
void ofxGuiInputField<Type>::generateDraw(){

	ofxGuiElement::generateDraw();

	generateText();
}


template<typename Type>
void ofxGuiInputField<Type>::generateText(){
	std::string valStr = input;
	textMesh = getTextMesh(getName(), textPadding, getShape().height / 2 + 4);
	textMesh.append(getTextMesh(valStr, getShape().width - textPadding - getTextBoundingBox(valStr,0,0).width, getShape().height / 2 + 4));
}

template<typename Type>
void ofxGuiInputField<Type>::render(){
	ofxGuiElement::render();

	if(hasFocus){
		drawFocusedBB();

		if(hasSelectedText()){
			drawSelectedArea();
		}else{
			drawCursor();
		}
	}

	drawMesh();
}

template<typename Type>
bool ofxGuiInputField<Type>::hasSelectedText(){
	return selectStartPos != selectEndPos;
}

template<typename Type>
void ofxGuiInputField<Type>::drawMesh(){
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(textColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofColor c = ofGetStyle().color;
	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

template<typename Type>
void ofxGuiInputField<Type>::drawSelectedArea(){
	ofPushStyle();
	ofSetColor(fillColor);
	ofFill();
	ofDrawRectangle( selectStartX, 1, selectionWidth, getShape().height-2 );
	ofPopStyle();
}

template<typename Type>
void ofxGuiInputField<Type>::drawCursor(){
	ofPushStyle();
	ofSetColor(textColor);
	ofDrawLine( selectStartX, 0, selectStartX, getShape().getHeight() );
	ofPopStyle();
}

template<typename Type>
void ofxGuiInputField<Type>::drawFocusedBB(){
	ofPushStyle();
	ofSetColor(textColor);
	ofDrawLine( selectStartX, 0, selectStartX, getShape().getHeight() );
	ofPopStyle();
}

template<typename Type>
bool ofxGuiInputField<Type>::setValue(float mx, float my, bool bCheck){
	return false;
}

template<typename Type>
ofAbstractParameter & ofxGuiInputField<Type>::getParameter(){
	return value;
}

template<typename Type>
void ofxGuiInputField<Type>::parseInput(){
	bChangedInternally = true;
	Type tmpVal = ofToFloat(input);
	if(tmpVal < getMin()){
		tmpVal = getMin();
	}else if(tmpVal > getMax()){
		tmpVal = getMax();
	}
	value = tmpVal;
}

template<>
void ofxGuiInputField<std::string>::parseInput(){
	bChangedInternally = true;
	value = input;
}

template<typename Type>
void ofxGuiInputField<Type>::valueChanged(Type & value){
	if(bChangedInternally){
		bChangedInternally = false;
		inputWidth = getTextBoundingBox(input,0,0).width;
	}else{
		input = ofToString(value);
		inputWidth = getTextBoundingBox(input,0,0).width;
		if(hasFocus){
			int cursorPos = input.size();
			calculateSelectionArea(cursorPos,cursorPos);
		}
	}
	setNeedsRedraw();
}

template<typename Type>
void ofxGuiInputField<Type>::leaveFocus(){
	pressCounter = 0;
	input = ofToString(value);
	inputWidth = getTextBoundingBox(input,0,0).width;
	hasFocus = false;
	setNeedsRedraw();
}

template<typename Type>
std::string ofxGuiInputField<Type>::getClassType(){
	return "input";
}

template<typename Type>
std::vector<std::string> ofxGuiInputField<Type>::getClassTypes(){
	std::vector<std::string> types = ofxGuiElement::getClassTypes();
	types.push_back(getClassType());
	return types;
}

template class ofxGuiInputField<int8_t>;
template class ofxGuiInputField<uint8_t>;
template class ofxGuiInputField<int16_t>;
template class ofxGuiInputField<uint16_t>;
template class ofxGuiInputField<int32_t>;
template class ofxGuiInputField<uint32_t>;
template class ofxGuiInputField<int64_t>;
template class ofxGuiInputField<uint64_t>;
template class ofxGuiInputField<float>;
template class ofxGuiInputField<double>;
template class ofxGuiInputField<std::string>;
