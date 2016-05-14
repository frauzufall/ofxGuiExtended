#include "ofxDOMFlexBoxLayout.h"
#include "ofxDOMLayoutHelper.h"
#include "ofMain.h"
#include "ofxGuiElement.h"

typedef ofxDOMLayoutHelper DOMLH;

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

ofxDOMFlexBoxLayout::ofxDOMFlexBoxLayout(DOM::Element* parent):
	DOM::_Layout<ofxDOMFlexBoxLayout>(parent){
}

ofxDOMFlexBoxLayout::~ofxDOMFlexBoxLayout(){
}

void ofxDOMFlexBoxLayout::doLayout(){

	if(!_isDoingLayout){

		if(_parent){

			// Prevent recursive calls to doLayout.
			_isDoingLayout = true;

			align(getFlexDirection(_parent));

			_isDoingLayout = false;

		}

	}

}

void ofxDOMFlexBoxLayout::align(FlexDirection direction){

	bool horizontal = direction == FlexDirection::ROW;

	float paddingHorizontal = DOMLH::getPaddingHorizontal(_parent);
	float paddingVertical = DOMLH::getPaddingVertical(_parent);

	float marginHorizontal = DOMLH::getMarginHorizontal(_parent);
	float marginVertical = DOMLH::getMarginVertical(_parent);

	float wParent = ofGetWidth();
	float hParent = ofGetHeight();
	if(_parent->parent()){
		wParent = _parent->getSizeByParent().x;
		hParent = _parent->getSizeByParent().y;
	}

	float totalWidth = DOMLH::getDesiredWidthStretched(_parent, wParent) - paddingHorizontal;
	float totalHeight = DOMLH::getDesiredHeightStretched(_parent, hParent) - paddingVertical;

//	if(ofxGuiElement* el = dynamic_cast<ofxGuiElement*>(_parent)){
//		cout << el->getName() << " total size: " << totalWidth << " " << totalHeight << endl;
//	}

	vector<vector<DOM::Element*>> lines;
	vector<vector<DOM::Element*>> noFlexItems;
	vector<vector<DOM::Element*>> flexItems;
	vector<vector<float>> flexItemsBasis;
	vector<float> totalSpaceMainAxis;

	float mainAxisSize =  horizontal ? totalWidth : totalHeight;
	float crossAxisSize = horizontal ? totalHeight : totalWidth;

	int linecount = 0;

	if(children().size() > 0){
		//newline
		lines.push_back(vector<DOM::Element*>());
		noFlexItems.push_back(vector<DOM::Element*>());
		flexItems.push_back(vector<DOM::Element*>());
		flexItemsBasis.push_back(vector<float>());
		totalSpaceMainAxis.push_back(mainAxisSize);
	}

	//sort children according to flex attribute and main size of children

	for(unsigned int i = 0; i < children().size(); i++){

		DOM::Element* element = children().at(i);
		float w, h;
		if(horizontal){
			w = DOMLH::getDesiredWidth(element, totalWidth);
			h = DOMLH::getDesiredHeight(element, totalHeight);
		}else{
			w = DOMLH::getDesiredWidth(element, totalWidth);
			h = DOMLH::getDesiredHeight(element, totalHeight);
		}

		float elementMainSize = horizontal ? w : h;
		float elementCrossSize = horizontal ? h : w;

		if(element){
			if(elementFlexing(element)){

//				element->setSizeByParent(totalWidth, totalHeight);

				// set to minimal size on main axis
				if(horizontal){
					element->setSizeByParent(elementMainSize + DOMLH::getMarginHorizontal(element), elementCrossSize + DOMLH::getMarginVertical(element));
					element->setLayoutSize(elementMainSize, elementCrossSize, true);
					elementMainSize = element->getWidth() + DOMLH::getMarginHorizontal(element);
				}else {
					element->setSizeByParent(elementCrossSize + DOMLH::getMarginHorizontal(element), elementMainSize + DOMLH::getMarginVertical(element));
					element->setLayoutSize(elementCrossSize, elementMainSize, true);
					elementMainSize = element->getHeight() + DOMLH::getMarginVertical(element);
				}

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

				// add to new line if it does not fit and flex-wrap is on
				if((int)totalSpaceMainAxis.at(linecount) - (int)elementMainSize < 0){
					FlexWrap _wrap = getFlexWrap(_parent);
					if(_wrap == FlexWrap::NOWRAP || i == 0){
						//no new line
					}else{
						//new line
						linecount++;
						lines.push_back(vector<DOM::Element*>());
						flexItems.push_back(vector<DOM::Element*>());
						flexItemsBasis.push_back(vector<float>());
						totalSpaceMainAxis.push_back(mainAxisSize);
					}
				}

				lines.at(linecount).push_back(element);
				totalSpaceMainAxis.at(linecount) -= elementMainSize;
			}else {
				//set an absolute positioned element to its desired independent size
				if(DOMLH::elementAbsolutePositioned(element)){

					element->setLayoutSize(w, h);

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

		if(partscount > 0){

			float partsize = totalSpaceMainAxis.at(i)/partscount;

			totalSpaceMainAxis.at(i) = 0;

			for(unsigned int j = 0; j < flexItems.at(i).size(); j++){
				DOM::Element* element = flexItems.at(i).at(j);
				if(horizontal){
					element->setSizeByParent(flexItemsBasis.at(i).at(j)*partsize, element->getSizeByParent().y);
					setLayoutWidthMinusMargin(element, flexItemsBasis.at(i).at(j)*partsize);
				}else{
					element->setSizeByParent(element->getSizeByParent().x, flexItemsBasis.at(i).at(j)*partsize);
					setLayoutHeightMinusMargin(element, flexItemsBasis.at(i).at(j)*partsize);
				}
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
			float elementCrossSize = horizontal ?
						e->getHeight()+DOMLH::getMarginVertical(e) :
						e->getWidth()+DOMLH::getMarginHorizontal(e);
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
			float elementCrossSize = horizontal ?
						e->getHeight()+DOMLH::getMarginVertical(e) :
						e->getWidth()+DOMLH::getMarginHorizontal(e);

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
//			setHeightInLayoutAddPadding_parent, newCrossAxisSize);
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

	totalWidth += paddingHorizontal;
	totalHeight += paddingVertical;

	float parentPaddingLeft = DOMLH::getPaddingLeft(_parent);
	float parentPaddingTop = DOMLH::getPaddingTop(_parent);

	float currentMainPos = 0;
	float currentCrossPos = spacingCrossAxisStart;
	currentCrossPos += horizontal ? parentPaddingTop : parentPaddingLeft;

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
					element->setSizeByParent(element->getSizeByParent().x, lineSizes.at(i));
					setLayoutHeightMinusMargin(element, lineSizes.at(i));
				}else{
					element->setSizeByParent(lineSizes.at(i), element->getSizeByParent().y);
					setLayoutWidthMinusMargin(element, lineSizes.at(i));
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
				DOMLH::setPosition(element, ofPoint(elementMainPos, elementCrossPos));
			}else{
				DOMLH::setPosition(element, ofPoint(elementCrossPos, elementMainPos));
			}

			totalWidth = max(totalWidth, element->getShape().getRight()+DOMLH::getMarginRight(element)+DOMLH::getPaddingRight(_parent));
			totalHeight = max(totalHeight, element->getShape().getBottom()+DOMLH::getMarginBottom(element)+DOMLH::getPaddingBottom(_parent));

			currentMainPos += elementMainSize + spacingMainAxisBetween;

		}

		currentCrossPos += lineSizes.at(i) + spacingCrossAxisBetween;

	}


	//make sure parent element contains all child elements on the main axis
//	maxX += DOMLH::getPaddingRight(_parent);
//	maxY += DOMLH::getPaddingBottom(_parent);
//	if(horizontal){
//		_parent->setLayoutSize(max(maxX,_parent->getWidth()), max(maxY,_parent->getHeight()));
//	}else{
//		_parent->setLayoutSize(max(maxX,_parent->getWidth()), max(maxY,_parent->getHeight()));
//	}

//	if(ofxGuiElement* el = dynamic_cast<ofxGuiElement*>(_parent)){
//		cout << el->getName() << " total size end: " << totalWidth << " " << totalHeight << endl;
//	}

	_parent->setLayoutSize(totalWidth, totalHeight, false);
	_parent->setNeedsRedraw();

}

bool ofxDOMFlexBoxLayout::elementFlexing(DOM::Element* e){
	if(!e->getVisible().get()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<DOM::LayoutPosition>("position") == DOM::LayoutPosition::ABSOLUTE){
			return false;
		}
	}
	return true;
}

float ofxDOMFlexBoxLayout::getWidthPlusMargin(DOM::Element* e){
	return e->getWidth() + DOMLH::getMarginHorizontal(e);
}

void ofxDOMFlexBoxLayout::setLayoutWidthMinusMargin(DOM::Element* e, float width){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	width -= DOMLH::getMarginHorizontal(e);
	e->setLayoutWidth(width);
}

float ofxDOMFlexBoxLayout::getHeightPlusMargin(DOM::Element* e){
	return e->getHeight() + DOMLH::getMarginVertical(e);
}

void ofxDOMFlexBoxLayout::setLayoutHeightMinusMargin(DOM::Element* e, float height){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	height -= DOMLH::getMarginVertical(e);
	e->setLayoutHeight(height);
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
