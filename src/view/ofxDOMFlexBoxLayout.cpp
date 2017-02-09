#include "ofxDOMFlexBoxLayout.h"
#include "ofxDOMLayoutHelper.h"
#include "ofMain.h"
//#include "ofxGuiElement.h"

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


	vector<FlexBoxLine> lines;

	float mainAxisSize =  horizontal ? totalWidth : totalHeight;
	float crossAxisSize = horizontal ? totalHeight : totalWidth;

	int linecount = 0;

	if(children().size() > 0){
		//newline
		lines.push_back(FlexBoxLine(mainAxisSize));
	}

	//sort children according to flex attribute and main size of children

	bool firstChild = true;
	for(unsigned int i = 0; i < children().size(); i++){

		DOM::Element* element = children().at(i);

		FlexBoxObject obj;
		obj.origin = element;
		obj.shape.width = DOMLH::getDesiredWidth(element, totalWidth);
		obj.shape.height = DOMLH::getDesiredHeight(element, totalHeight);

		float elementMainSize = horizontal ? obj.shape.width : obj.shape.height;

		if(elementFlexing(element)){

			//if element is flexible, add it to the current line and save the items flex basis
			// TODO check if it fits in with minimal size
			if(element->hasAttribute("_flex")){
				std::string flexvalstr = element->getAttribute<std::string>("_flex");
				if(flexvalstr == "auto"){
					obj.flex = true;
					obj.flexBasis = 1;
				}
				if(isFloat(ofTrim(flexvalstr))){
					float _flexval = ofToFloat(flexvalstr);
					if(_flexval > 0){
						obj.flex = true;
						obj.flexBasis = _flexval;
					}
				}
			}

			if(!obj.flex){
				// not flexible or no valid flex attribute, not flexing on main axis

				element->setSizeByParent(obj.shape.width + DOMLH::getMarginHorizontal(element), obj.shape.height + DOMLH::getMarginVertical(element));
				element->setLayoutSize(obj.shape.width, obj.shape.height, true);
				if(horizontal){
					elementMainSize = element->getWidth()+DOMLH::getMarginHorizontal(element);
					obj.shape.width = elementMainSize;
				}else {
					elementMainSize = element->getHeight()+DOMLH::getMarginVertical(element);
					obj.shape.height = elementMainSize;
				}
			}

			// add to new line if it does not fit and flex-wrap is on
			if((int)lines.at(linecount).totalSpaceMainAxis - (int)elementMainSize < 0){
				FlexWrap _wrap = getFlexWrap(_parent);
				if(_wrap == FlexWrap::NOWRAP || firstChild){
					//no new line
				}else{
					//new line
					linecount++;
					lines.push_back(FlexBoxLine(mainAxisSize));
				}
			}

			// add item to line vectors and save
			lines.at(linecount).items.push_back(obj);
			if(!obj.flex){
				// subtract main element size from main axis free space
				lines.at(linecount).totalSpaceMainAxis -= elementMainSize;
			}

			firstChild = false;


		}else {
			//set an absolute positioned element to its desired independent size
			if(DOMLH::elementAbsolutePositioned(element)){

				element->setLayoutSize(obj.shape.width, obj.shape.height);

			}
		}
	}

	//compute main size of flex items if they are flexible

	for(FlexBoxLine& line : lines){
		int partscount = 0;
		for(FlexBoxObject& obj : line.items){
			partscount += obj.flexBasis;
		}

		if(partscount > 0){

			float partsize = line.totalSpaceMainAxis/partscount;

			line.totalSpaceMainAxis = 0;

			for(FlexBoxObject& obj : line.items){
				if(obj.flex){
					if(horizontal){
						obj.origin->setSizeByParent(obj.flexBasis*partsize, obj.origin->getSizeByParent().y);
						obj.shape.width = obj.flexBasis*partsize - DOMLH::getMarginHorizontal(obj.origin);
					}else{
						obj.origin->setSizeByParent(obj.origin->getSizeByParent().x, obj.flexBasis*partsize);
						obj.shape.height = obj.flexBasis*partsize - DOMLH::getMarginVertical(obj.origin);
					}
				}
			}
		}
	}

	//set cross size of items if they stretch

	AlignItems alignItems = getAlignItems(_parent);
	AlignContent alignContent = getAlignContent(_parent);

	float totalSpaceCrossAxis = crossAxisSize;

	for(FlexBoxLine& line : lines){

		float lineSize = 0;
		for(FlexBoxObject& obj : line.items){
			float elementCrossSize = horizontal ?
						DOMLH::getDesiredHeight(obj.origin, totalHeight)+DOMLH::getMarginVertical(obj.origin) :
						DOMLH::getDesiredWidth(obj.origin, totalWidth)+DOMLH::getMarginHorizontal(obj.origin);
			if(elementCrossSize > lineSize){
				lineSize = elementCrossSize;
			}
			if(alignContent == AlignContent::STRETCH){
				lineSize = max(lineSize, crossAxisSize / lines.size());
			}
		}
		totalSpaceCrossAxis -= lineSize;
		line.sizeCrossAxis = lineSize;
	}


	// check if lines are not big enough to fit in all elements minimal cross size
	for(FlexBoxLine& line : lines){

		float lineSize = line.sizeCrossAxis;
		for(FlexBoxObject& obj : line.items){
			AlignSelf alignSelf = getAlignSelf(obj.origin);
			float mainSize = horizontal? obj.origin->getWidth() : obj.origin->getHeight();
			if(obj.flex){
				mainSize = horizontal? obj.shape.width : obj.shape.height;
			}
			if(alignSelf == AlignSelf::STRETCH ||
			  ((alignSelf == AlignSelf::AUTO) && (alignItems == AlignItems::STRETCH))){
				if(horizontal){
					obj.origin->setSizeByParent(obj.origin->getSizeByParent().x, lineSize);
					obj.origin->setLayoutSize(mainSize, lineSize - DOMLH::getMarginVertical(obj.origin));
				}else{
					obj.origin->setSizeByParent(lineSize, obj.origin->getSizeByParent().y);
					obj.origin->setLayoutSize(lineSize - DOMLH::getMarginHorizontal(obj.origin), mainSize);
				}
			}else{
				if(horizontal){
					obj.origin->setLayoutSize(mainSize, DOMLH::getDesiredHeight(obj.origin, lineSize));
				}else{
					obj.origin->setLayoutSize(DOMLH::getDesiredWidth(obj.origin, lineSize), mainSize);
				}
			}
			float elementCrossSize = horizontal ?
						obj.origin->getHeight()+DOMLH::getMarginVertical(obj.origin) :
						obj.origin->getWidth()+DOMLH::getMarginHorizontal(obj.origin);

			if(elementCrossSize > lineSize){
				lineSize = elementCrossSize;
			}
		}
		line.sizeCrossAxis = lineSize;
	}

	float newCrossAxisSize = 0;
	for(FlexBoxLine& line : lines){
		newCrossAxisSize += line.sizeCrossAxis;
	}
	if(newCrossAxisSize > crossAxisSize){
		totalSpaceCrossAxis = 0;
	}

	//take care of empty space on cross axis
	int spacingCrossAxisStart = 0;
	int spacingCrossAxisBetween = 0;
	if(lines.size() > 1){
		if(totalSpaceCrossAxis > 0){
			switch(alignContent){
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
				default:break;
			}
		}
	}else{
		if(lines.size()>0){
			lines.at(0).sizeCrossAxis = max(lines.at(0).sizeCrossAxis,crossAxisSize);
		}
	}

	totalWidth += paddingHorizontal;
	totalHeight += paddingVertical;

	float parentPaddingLeft = DOMLH::getPaddingLeft(_parent);
	float parentPaddingTop = DOMLH::getPaddingTop(_parent);

	float currentMainPos = 0;
	float currentCrossPos = spacingCrossAxisStart;
	currentCrossPos += horizontal ? parentPaddingTop : parentPaddingLeft;

	for(FlexBoxLine& line : lines){

		//take care of empty space on main axis
		int spacingMainAxisStart = horizontal ? parentPaddingLeft : parentPaddingTop;
		int spacingMainAxisBetween = 0;
		if(line.totalSpaceMainAxis > 0){
			switch(getJustifyContent(_parent)){
				case JustifyContent::CENTER:
					spacingMainAxisStart += line.totalSpaceMainAxis/2;
					break;
				case JustifyContent::FLEX_END:
					spacingMainAxisStart += line.totalSpaceMainAxis;
					break;
				case JustifyContent::SPACE_AROUND:
					spacingMainAxisStart += line.totalSpaceMainAxis/(line.items.size()+1);
					spacingMainAxisBetween = spacingMainAxisStart;
					break;
				case JustifyContent::SPACE_BETWEEN:
					spacingMainAxisBetween = line.totalSpaceMainAxis/(line.items.size()-1);
					break;
				default:break;
			}
		}

		currentMainPos = spacingMainAxisStart;

		for(FlexBoxObject& obj : line.items){

			// set cross size of item

			DOM::Element* element = obj.origin;
			AlignSelf alignSelf = getAlignSelf(element);
			if(alignSelf == AlignSelf::STRETCH ||
			  ((alignSelf == AlignSelf::AUTO) && (alignItems == AlignItems::STRETCH))){
				if(horizontal){
					element->setSizeByParent(element->getSizeByParent().x, line.sizeCrossAxis);
					setLayoutHeightMinusMargin(element, line.sizeCrossAxis);
				}else{
					element->setSizeByParent(line.sizeCrossAxis, element->getSizeByParent().y);
					setLayoutWidthMinusMargin(element, line.sizeCrossAxis);
				}
			}/*else{
				if(horizontal){
					element->setLayoutHeight(DOMLH::getDesiredHeight(element, lineSizes.at(i)));
				}else{
					element->setLayoutWidth(DOMLH::getDesiredWidth(element, lineSizes.at(i)));
				}
			}*/

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
					elementCrossPos += line.sizeCrossAxis-elementCrossSize;
					break;
				case AlignItems::CENTER:
					elementCrossPos += (line.sizeCrossAxis-elementCrossSize)/2.;
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

		currentCrossPos += line.sizeCrossAxis + spacingCrossAxisBetween;

	}

	_parent->setLayoutSize(totalWidth, totalHeight, false);
	_parent->setNeedsRedraw();

}

bool ofxDOMFlexBoxLayout::elementFlexing(DOM::Element* e){
	if(!e->getVisible().get()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<DOM::LayoutPosition>("position") == DOM::LayoutPosition::POSITION_ABSOLUTE){
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
