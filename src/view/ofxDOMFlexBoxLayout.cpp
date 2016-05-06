#include "ofxDOMFlexBoxLayout.h"
#include "../ofxGuiElement.h"
#include "JsonConfigParser.h"


bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

bool isFloat( std::string myString ) {
	std::istringstream iss(myString);
	float f;
	iss >> noskipws >> f; // noskipws considers leading whitespace invalid
	// Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();
}

ofxDOMFlexBoxLayout::ofxDOMFlexBoxLayout(DOM::Element* parent, DOM::Orientation orientation):
	ofxDOMBoxLayout(parent, orientation){
}

ofxDOMFlexBoxLayout::~ofxDOMFlexBoxLayout(){
}

void ofxDOMFlexBoxLayout::doLayout(){

	if(!_isDoingLayout){

		// Prevent recursive calls to doLayout.
		_isDoingLayout = true;

		if(_parent){
			if(_parent->parent() == nullptr){
				_parent->setWidthInLayout(getDesiredWidth(_parent));
				_parent->setHeightInLayout(getDesiredHeight(_parent));
			}
		}

		if (_parent && children().size() > 0){

			align(getFlexDirection(_parent));

		}

		_isDoingLayout = false;

	}

}

void ofxDOMFlexBoxLayout::align(FlexDirection direction){

	bool horizontal = direction == FlexDirection::ROW;

//	ofxGuiElement* baseg= dynamic_cast<ofxGuiElement*>(_parent);
//	if(baseg){
//		if(baseg->getName() == "default"){
//			cout << "aligning elements in " << baseg->getName() << " of size " << baseg->getSize() << " minimal: " << baseg->getMinWidth() << " x " << baseg->getMinHeight() << endl;
//			cout << _parent->getWidth() - getPaddingHorizontal(_parent) << " " << _parent->getHeight() - getPaddingVertical(_parent) << endl;
//		}
//	}

	vector<vector<DOM::Element*>> lines;
	vector<vector<DOM::Element*>> noFlexItems;
	vector<vector<DOM::Element*>> flexItems;
	vector<vector<float>> flexItemsBasis;
	vector<float> totalSpaceMainAxis;
	float mainAxisSize, crossAxisSize;
	if(horizontal){
		mainAxisSize = _parent->getWidth() - getPaddingHorizontal(_parent);
		crossAxisSize = _parent->getHeight() - getPaddingVertical(_parent);
	}else{
		mainAxisSize = _parent->getHeight() - getPaddingVertical(_parent);
		crossAxisSize = _parent->getWidth() - getPaddingHorizontal(_parent);
	}
	int linecount = 0;

	if(children().size() > 0){
		//newline
		lines.push_back(vector<DOM::Element*>());
		noFlexItems.push_back(vector<DOM::Element*>());
		flexItems.push_back(vector<DOM::Element*>());
		flexItemsBasis.push_back(vector<float>());
		totalSpaceMainAxis.push_back(mainAxisSize);
	}

	//sort children according to flex attribute

	for(unsigned int i = 0; i < children().size(); i++){

		DOM::Element* element = children().at(i);
		float elementMainSize = horizontal ? getDesiredWidth(element) : getDesiredHeight(element);
		float elementCrossSize = horizontal ? getDesiredHeight(element) : getDesiredWidth(element);

		if(element){
			if(elementFlexing(element)){

				//if element is flexible, add it to the current line and save the items flex basis
				if(element->hasAttribute("_flex")){
					std::string flexval = element->getAttribute<std::string>("_flex");
					if(flexval == "auto"){
						lines.at(linecount).push_back(element);
						flexItems.at(linecount).push_back(element);
						flexItemsBasis.at(linecount).push_back(1);
						continue;
					}
					if(isFloat(ofTrim(flexval))){
						float intflexval = ofToFloat(flexval);
						if(intflexval > 0){
							lines.at(linecount).push_back(element);
							flexItems.at(linecount).push_back(element);
							flexItemsBasis.at(linecount).push_back(intflexval);
							continue;
						}
					}
				}

				// not flexible or no valid flex attribute, not flexing on main axis

				if((int)totalSpaceMainAxis.at(linecount) - (int)elementMainSize < 0){
					FlexWrap _wrap = getFlexWrap(_parent);
					if(_wrap == FlexWrap::NOWRAP || i == 0){
						//no new line
						if(horizontal){
							setWidthInLayoutAddPadding(_parent, mainAxisSize + elementMainSize-totalSpaceMainAxis.at(linecount));
						}else {
							setHeightInLayoutAddPadding(_parent, mainAxisSize + elementMainSize-totalSpaceMainAxis.at(linecount));
						}
					}else{
						//new line
						linecount++;
						lines.push_back(vector<DOM::Element*>());
						flexItems.push_back(vector<DOM::Element*>());
						flexItemsBasis.push_back(vector<float>());
						totalSpaceMainAxis.push_back(mainAxisSize);
					}
				}
				// set to minimal size on main axis
				if(horizontal){
					setLayoutWidthMinusMargin(element, elementMainSize);
				}else {
					setLayoutHeightMinusMargin(element, elementMainSize);
				}
				lines.at(linecount).push_back(element);
				totalSpaceMainAxis.at(linecount) -= elementMainSize;
			}else {
				//set an absolute positioned element to its desired independent size
				if(elementAbsolutePositioned(element)){
//					cout << "absolute element " << elementMainSize << " " << elementCrossSize << endl;
//					float w = element->getChildShape().getRight()+getPaddingRight(element);
//					float h = element->getChildShape().getBottom()+getPaddingBottom(element);
//					if(horizontal){
//						element->setSizeInLayout(max(w,elementMainSize), max(h,elementCrossSize));
//					}else {
//						element->setSizeInLayout(max(w,elementCrossSize), max(h,elementMainSize));
//					}

					if(element->layout()){
						if(element->layout()->isDoingLayout()){
							continue;
						}
					}

					if(horizontal){
						element->setSizeInLayout(elementMainSize, elementCrossSize);
					}else {
						element->setSizeInLayout(elementCrossSize, elementMainSize);
					}

					element->invalidateChildShape(false);

				}
			}
		}
	}

	//set main size of flex items if they are flexible

	for(unsigned int i = 0; i < flexItems.size(); i++){
		int partscount = 0;
		for(int parts : flexItemsBasis.at(i)){
			partscount += parts;
		}
		float partsize = totalSpaceMainAxis.at(i)/partscount;
		if(partscount > 0){
			totalSpaceMainAxis.at(i) = 0;
		}
		for(unsigned int j = 0; j < flexItems.at(i).size(); j++){
			DOM::Element* element = flexItems.at(i).at(j);
			if(horizontal){
				setLayoutWidthMinusMargin(element, flexItemsBasis.at(i).at(j)*partsize);
			}else{
				setLayoutHeightMinusMargin(element, flexItemsBasis.at(i).at(j)*partsize);
			}
		}
	}

	//set cross size of items if they stretch

	AlignItems alignItems = getAlignItems(_parent);

	vector<float> lineSizes;
	float totalSpaceCrossAxis = crossAxisSize;

	for(unsigned int i = 0; i < lines.size(); i++){

		float lineSize = 0;
		for(auto e : lines.at(i)){
			float elementCrossSize = horizontal ? getDesiredHeight(e) : getDesiredWidth(e);
			AlignSelf alignSelf = getAlignSelf(e);
			if(((alignSelf != AlignSelf::AUTO) && (alignSelf != AlignSelf::STRETCH)) ||
			  ((alignSelf == AlignSelf::AUTO) && (alignItems != AlignItems::STRETCH))){
				if(elementCrossSize > lineSize){
					lineSize = elementCrossSize;
				}
			}
		}
		totalSpaceCrossAxis -= lineSize;
		lineSizes.push_back(lineSize);
	}

	// count how many lines do not have a fixed size
	int zerolines = 0;
	for(int lineSize : lineSizes){
		if(lineSize == 0){
			zerolines++;
		}
	}

	// if there are lines without fixed height, take the remaining height of the parent
	// and share it between the lines without fixed height
	if(zerolines > 0){
		for(unsigned int i = 0; i < lineSizes.size(); i++){
			if(lineSizes[i] == 0){
				lineSizes[i] = totalSpaceCrossAxis / zerolines;
			}
		}
		totalSpaceCrossAxis = 0;
	}

	// check if lines are not big enough to fit in all elements minimal size
	for(unsigned int i = 0; i < lines.size(); i++){

		float lineSize = lineSizes.at(i);
		for(auto e : lines.at(i)){
			float elementCrossSize = horizontal ? getDesiredHeight(e) : getDesiredWidth(e);
			if(elementCrossSize > lineSize){
				lineSize = elementCrossSize;
			}
		}
		lineSizes.at(i) = lineSize;
	}

	float newCrossAxisSize = 0;
	for(int size : lineSizes){
		newCrossAxisSize += size;
	}
	if(newCrossAxisSize > crossAxisSize){
		totalSpaceCrossAxis = 0;
//		if(horizontal){
//			setHeightInLayoutAddPadding(_parent, newCrossAxisSize);
//		}else {
//			setWidthInLayoutAddPadding(_parent, newCrossAxisSize);
//		}
	}

	//take care of empty space on cross axis
	int spacingCrossAxisStart = 0;
	int spacingCrossAxisBetween = 0;
	if(lines.size() > 1){
		if(totalSpaceCrossAxis > 0){
			switch(getAlignContent(_parent)){
				case AlignContent::CENTER:
					spacingCrossAxisStart = totalSpaceCrossAxis/2;
					break;
				case AlignContent::FLEX_END:
					spacingCrossAxisStart = totalSpaceCrossAxis;
					break;
				case AlignContent::SPACE_AROUND:
					spacingCrossAxisStart = totalSpaceCrossAxis/(lines.size()+1);
					spacingCrossAxisBetween = spacingCrossAxisStart;
					break;
				case AlignContent::SPACE_BETWEEN:
					spacingCrossAxisBetween = totalSpaceCrossAxis/(lines.size()-1);
					break;
				case AlignContent::STRETCH:
					spacingCrossAxisBetween = totalSpaceCrossAxis/lines.size();
					break;
				default:break;
			}
		}
	}else{
		if(lines.size()>0){
			lineSizes.at(0) = max(lineSizes.at(0),crossAxisSize);
		}
	}

	float parentPaddingLeft = getPaddingLeft(_parent);
	float parentPaddingTop = getPaddingTop(_parent);

	float currentMainPos = 0;
	float currentCrossPos = spacingCrossAxisStart;
	currentCrossPos += horizontal ? parentPaddingTop : parentPaddingLeft;

	float maxX = 0, maxY = 0;

	for(unsigned int i = 0; i < lines.size(); i++){

		//take care of empty space on main axis
		int spacingMainAxisStart = horizontal ? parentPaddingLeft : parentPaddingTop;
		int spacingMainAxisBetween = 0;
		if(totalSpaceMainAxis.at(i) > 0){
			switch(getJustifyContent(_parent)){
				case JustifyContent::CENTER:
					spacingMainAxisStart += totalSpaceMainAxis.at(i)/2;
					break;
				case JustifyContent::FLEX_END:
					spacingMainAxisStart += totalSpaceMainAxis.at(i);
					break;
				case JustifyContent::SPACE_AROUND:
					spacingMainAxisStart += totalSpaceMainAxis.at(i)/(lines.at(i).size()+1);
					spacingMainAxisBetween = spacingMainAxisStart;
					break;
				case JustifyContent::SPACE_BETWEEN:
					spacingMainAxisBetween = totalSpaceMainAxis.at(i)/(lines.at(i).size()-1);
					break;
				default:break;
			}
		}

		currentMainPos = spacingMainAxisStart;

		for(unsigned int j = 0; j < lines.at(i).size(); j++){

			// set cross size of item

			DOM::Element* element = lines.at(i).at(j);
			AlignSelf alignSelf = getAlignSelf(element);
			if(alignSelf == AlignSelf::STRETCH ||
			  ((alignSelf == AlignSelf::AUTO) && (alignItems == AlignItems::STRETCH))){
				if(horizontal){
					setLayoutHeightMinusMargin(element, lineSizes.at(i));
				}else{
					setLayoutWidthMinusMargin(element, lineSizes.at(i));
				}
			}else {
				if(horizontal){
					setLayoutHeightMinusMargin(element, getDesiredHeight(element));
				}else{
					setLayoutWidthMinusMargin(element, getDesiredWidth(element));
				}
			}

			//align item

			float elementMainPos = currentMainPos;
			float elementCrossPos = currentCrossPos;
			float elementMainSize = horizontal ? getWidthPlusMargin(element) : getHeightPlusMargin(element);
			float elementCrossSize = horizontal ? getHeightPlusMargin(element) : getWidthPlusMargin(element);

			//align item on cross axis

			AlignItems alignItem = alignItems;
			if(alignSelf != AlignSelf::AUTO){
				switch(alignSelf){
					case AlignSelf::CENTER:
						alignItem = AlignItems::CENTER;
						break;
					case AlignSelf::STRETCH:
					case AlignSelf::FLEX_START:
						alignItem = AlignItems::FLEX_START;
						break;
					case AlignSelf::FLEX_END:
						alignItem = AlignItems::FLEX_END;
						break;
					default:
						break;
				}
			}

			switch(alignItem){
				case AlignItems::FLEX_END:
					elementCrossPos += lineSizes.at(i)-elementCrossSize;
					break;
				case AlignItems::CENTER:
					elementCrossPos += (lineSizes.at(i)-elementCrossSize)/2.;
					break;
				default:
					break;
			}

			//set final element position
			if(horizontal){
				setPosition(element, ofPoint(elementMainPos, elementCrossPos));
			}else{
				setPosition(element, ofPoint(elementCrossPos, elementMainPos));
			}

			maxX = max(maxX, element->getShape().getRight()+getMarginRight(element));
			maxY = max(maxY, element->getShape().getBottom()+getMarginBottom(element));

			currentMainPos += elementMainSize + spacingMainAxisBetween;

		}

		currentCrossPos += lineSizes.at(i) + spacingCrossAxisBetween;

	}


	//make sure parent element contains all child elements on the main axis
	maxX += getPaddingRight(_parent);
	maxY += getPaddingBottom(_parent);
	if(horizontal){
		_parent->setSizeInLayout(max(maxX,_parent->getWidth()), max(maxY,_parent->getHeight()));
	}else{
		_parent->setSizeInLayout(max(maxX,_parent->getWidth()), max(maxY,_parent->getHeight()));
	}

}

bool ofxDOMFlexBoxLayout::elementFlexing(DOM::Element* e){
	if(e->isHidden()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<DOM::LayoutPosition>("position") == DOM::LayoutPosition::ABSOLUTE){
			return false;
		}
	}
	return true;
}

bool ofxDOMFlexBoxLayout::elementAbsolutePositioned(DOM::Element* e){
	if(e->isHidden()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<DOM::LayoutPosition>("position") == DOM::LayoutPosition::ABSOLUTE){
			return true;
		}
	}
	return false;
}



float ofxDOMFlexBoxLayout::getWidthPlusMargin(DOM::Element* e){
	return e->getWidth() + getMarginHorizontal(e);
}

float ofxDOMFlexBoxLayout::getDesiredWidth(DOM::Element* e){
	float res = e->getMinWidth() + getPaddingHorizontal(e);

	if(e->hasAttribute("_width")){
		std::string widthstr = e->getAttribute<std::string>("_width");
		if(ofIsStringInString(widthstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(widthstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				if(e->parent()){
					return (e->parent()->getWidth()-getPaddingHorizontal(e->parent()))*amount;
				}else {
					return ofGetWindowWidth()*amount;
				}
			}
		}else {
			return ofToFloat(widthstr);
		}
	}

	res += getMarginHorizontal(e);

	return res;
}

void ofxDOMFlexBoxLayout::setWidthInLayoutAddPadding(DOM::Element* e, float width){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	width += getPaddingHorizontal(e);
	e->setWidthInLayout(width);
}


void ofxDOMFlexBoxLayout::setLayoutWidthMinusMargin(DOM::Element* e, float width){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	width -= getMarginHorizontal(e);
	e->setWidthInLayout(width);
}

float ofxDOMFlexBoxLayout::getHeightPlusMargin(DOM::Element* e){
	return e->getHeight() + getMarginVertical(e);
}

float ofxDOMFlexBoxLayout::getDesiredHeight(DOM::Element* e){
	float res = e->getMinHeight() + getPaddingVertical(e);

	if(e->hasAttribute("_height")){
		std::string heightstr = e->getAttribute<std::string>("_height");
		if(ofIsStringInString(heightstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(heightstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				if(e->parent()){
					return (e->parent()->getHeight() - getPaddingVertical(e->parent()))*amount;
				}else {
					return ofGetWindowHeight()*amount;
				}
			}
		}else {
			return ofToFloat(heightstr);
		}
	}

	res += getMarginVertical(e);

	return res;
}

void ofxDOMFlexBoxLayout::setHeightInLayoutAddPadding(DOM::Element* e, float height){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	height += getPaddingVertical(e);
	e->setHeightInLayout(height);
}

void ofxDOMFlexBoxLayout::setLayoutHeightMinusMargin(DOM::Element* e, float height){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	height -= getMarginVertical(e);
	e->setHeightInLayout(height);
}

float ofxDOMFlexBoxLayout::getMarginHorizontal(DOM::Element *e){
	return getMarginLeft(e) + getMarginRight(e);
}

float ofxDOMFlexBoxLayout::getMarginVertical(DOM::Element *e){
	return getMarginTop(e) + getMarginBottom(e);
}

float ofxDOMFlexBoxLayout::getMarginLeft(DOM::Element *e){
	if(e->hasAttribute("_margin-left")){
		return ofToFloat(e->getAttribute<std::string>("_margin-left"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getMarginRight(DOM::Element *e){
	if(e->hasAttribute("_margin-right")){
		return ofToFloat(e->getAttribute<std::string>("_margin-right"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getMarginTop(DOM::Element *e){
	if(e->hasAttribute("_margin-top")){
		return ofToFloat(e->getAttribute<std::string>("_margin-top"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getMarginBottom(DOM::Element *e){
	if(e->hasAttribute("_margin-bottom")){
		return ofToFloat(e->getAttribute<std::string>("_margin-bottom"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getPaddingHorizontal(DOM::Element *e){
	return getPaddingLeft(e) + getPaddingRight(e);
}

float ofxDOMFlexBoxLayout::getPaddingVertical(DOM::Element *e){
	return getPaddingTop(e) + getPaddingBottom(e);
}

float ofxDOMFlexBoxLayout::getPaddingLeft(DOM::Element *e){
	if(e->hasAttribute("_padding-left")){
		return ofToFloat(e->getAttribute<std::string>("_padding-left"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getPaddingRight(DOM::Element *e){
	if(e->hasAttribute("_padding-right")){
		return ofToFloat(e->getAttribute<std::string>("_padding-right"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getPaddingTop(DOM::Element *e){
	if(e->hasAttribute("_padding-top")){
		return ofToFloat(e->getAttribute<std::string>("_padding-top"));
	}
	return 0;
}

float ofxDOMFlexBoxLayout::getPaddingBottom(DOM::Element *e){
	if(e->hasAttribute("_padding-bottom")){
		return ofToFloat(e->getAttribute<std::string>("_padding-bottom"));
	}
	return 0;
}

void ofxDOMFlexBoxLayout::setPosition(DOM::Element* e, ofPoint p){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	p.x += getMarginLeft(e);
	p.y += getMarginTop(e);
	e->setPosition(p);
}

ofxDOMFlexBoxLayout::FlexDirection ofxDOMFlexBoxLayout::getFlexDirection(DOM::Element *e, FlexDirection defaultVal){
	if(e->hasAttribute("_flex-direction")){
		std::string val = e->getAttribute<std::string>("_flex-direction");
		if(val == "row"){
			return FlexDirection::ROW;
		}
		if(val == "column"){
			return FlexDirection::COLUMN;
		}
	}
	return defaultVal;
}

ofxDOMFlexBoxLayout::FlexWrap ofxDOMFlexBoxLayout::getFlexWrap(DOM::Element *e, FlexWrap defaultVal){
	if(e->hasAttribute("_flex-wrap")){
		std::string val = e->getAttribute<std::string>("_flex-wrap");
		if(val == "nowrap"){
			return FlexWrap::NOWRAP;
		}
		if(val == "wrap"){
			return FlexWrap::WRAP;
		}
	}
	return defaultVal;
}

ofxDOMFlexBoxLayout::JustifyContent ofxDOMFlexBoxLayout::getJustifyContent(DOM::Element *e, JustifyContent defaultVal){
	if(e->hasAttribute("_justify-content")){
		std::string val = e->getAttribute<std::string>("_justify-content");
		if(val == "flex-start"){
			return JustifyContent::FLEX_START;
		}
		if(val == "flex-end"){
			return JustifyContent::FLEX_END;
		}
		if(val == "center"){
			return JustifyContent::CENTER;
		}
		if(val == "space-between"){
			return JustifyContent::SPACE_BETWEEN;
		}
		if(val == "space-around"){
			return JustifyContent::SPACE_AROUND;
		}
	}
	return defaultVal;
}

ofxDOMFlexBoxLayout::AlignItems ofxDOMFlexBoxLayout::getAlignItems(DOM::Element *e, AlignItems defaultVal){
	if(e->hasAttribute("_align-items")){
		std::string val = e->getAttribute<std::string>("_align-items");
		if(val == "flex-start"){
			return AlignItems::FLEX_START;
		}
		if(val == "flex-end"){
			return AlignItems::FLEX_END;
		}
		if(val == "center"){
			return AlignItems::CENTER;
		}
		if(val == "stretch"){
			return AlignItems::STRETCH;
		}
	}
	return defaultVal;
}

ofxDOMFlexBoxLayout::AlignContent ofxDOMFlexBoxLayout::getAlignContent(DOM::Element *e, AlignContent defaultVal){
	if(e->hasAttribute("_align-content")){
		std::string val = e->getAttribute<std::string>("_align-content");
		if(val == "stretch"){
			return AlignContent::STRETCH;
		}
		if(val == "flex-start"){
			return AlignContent::FLEX_START;
		}
		if(val == "flex-end"){
			return AlignContent::FLEX_END;
		}
		if(val == "center"){
			return AlignContent::CENTER;
		}
		if(val == "space-between"){
			return AlignContent::SPACE_BETWEEN;
		}
		if(val == "space-around"){
			return AlignContent::SPACE_AROUND;
		}
	}
	return defaultVal;
}

ofxDOMFlexBoxLayout::AlignSelf ofxDOMFlexBoxLayout::getAlignSelf(DOM::Element *e, AlignSelf defaultVal){
	if(e->hasAttribute("_align-self")){
		std::string val = e->getAttribute<std::string>("_align-self");
		if(val == "auto"){
			return AlignSelf::AUTO;
		}
		if(val == "stretch"){
			return AlignSelf::STRETCH;
		}
		if(val == "flex-start"){
			return AlignSelf::FLEX_START;
		}
		if(val == "flex-end"){
			return AlignSelf::FLEX_END;
		}
		if(val == "center"){
			return AlignSelf::CENTER;
		}
	}
	return defaultVal;
}
