#include "FlexBoxLayout.h"
#include "ofxBaseGui.h"
#include "JsonConfigParser.h"

namespace ofx {
namespace DOM {

bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

FlexBoxLayout::FlexBoxLayout(Element* parent, Orientation orientation):
	BoxLayout(parent, orientation){
}

FlexBoxLayout::~FlexBoxLayout(){
}

void FlexBoxLayout::doLayout(){

	if(!_isDoingLayout){

		// Prevent recursive calls to doLayout.
		_isDoingLayout = true;

		if(_parent){
			if(_parent->parent() == nullptr){
				_parent->setWidth(getDesiredWidth(_parent));
				_parent->setHeight(getDesiredHeight(_parent));
			}
		}

		if (_parent && children().size() > 0){

			align(getFlexDirection(_parent));

		}

		_isDoingLayout = false;

	}

}

void FlexBoxLayout::align(FlexDirection direction){

	bool horizontal = direction == FlexDirection::ROW;

//	ofxBaseGui* baseg= dynamic_cast<ofxBaseGui*>(_parent);
//	if(baseg){
//		if(baseg->getName() == "Settings")
//		cout << "aligning elements in " << baseg->getName() << " of size " << baseg->getSize() << " minimal: " << baseg->getMinWidth() << " x " << baseg->getMinHeight() << endl;
//	}

	vector<vector<Element*>> lines;
	vector<vector<Element*>> noFlexItems;
	vector<vector<Element*>> flexItems;
	vector<vector<int>> flexItemsBasis;
	vector<float> totalSpaceMainAxis;
	float mainAxisSize = horizontal ? _parent->getWidth() : _parent->getHeight();
	float crossAxisSize = horizontal ? _parent->getHeight() : _parent->getWidth();
	int linecount = 0;

	if(children().size() > 0){
		//newline
		lines.push_back(vector<Element*>());
		noFlexItems.push_back(vector<Element*>());
		flexItems.push_back(vector<Element*>());
		flexItemsBasis.push_back(vector<int>());
		totalSpaceMainAxis.push_back(mainAxisSize);
	}

	//sort children according to flex attribute

	for(unsigned int i = 0; i < children().size(); i++){

		Element* element = children().at(i);
		float elementMainSize = horizontal ? getDesiredWidth(element) : getDesiredHeight(element);
		float elementCrossSize = horizontal ? getDesiredHeight(element) : getDesiredWidth(element);

		if(element){
			if(elementFlexing(element)){
				if(element->hasAttribute("_flex")){
					std::string flexval = element->getAttribute<std::string>("_flex");
					if(flexval == "auto"){
						lines.at(linecount).push_back(element);
						flexItems.at(linecount).push_back(element);
						flexItemsBasis.at(linecount).push_back(1);
						continue;
					}
					if(isInteger(flexval)){
						int intflexval = ofToInt(flexval);
						if(intflexval > 0){
							lines.at(linecount).push_back(element);
							flexItems.at(linecount).push_back(element);
							flexItemsBasis.at(linecount).push_back(intflexval);
							continue;
						}
					}
				}
				if((int)totalSpaceMainAxis.at(linecount) - (int)elementMainSize < 0){
					//newline
					if(i!= 0){
						linecount++;
						lines.push_back(vector<Element*>());
						flexItems.push_back(vector<Element*>());
						flexItemsBasis.push_back(vector<int>());
						totalSpaceMainAxis.push_back(mainAxisSize);
					}else{
						if(horizontal){
							setWidthInLayout(_parent, elementMainSize);
						}else {
							setHeightInLayout(_parent, elementMainSize);
						}

					}
				}
				// not flexing on main axis, set to minimal size on main axis
				if(horizontal){
					setWidthInLayout(element, elementMainSize);
				}else {
					setHeightInLayout(element, elementMainSize);
				}
				lines.at(linecount).push_back(element);
				totalSpaceMainAxis.at(linecount) -= elementMainSize;
			}else {
				//set an absolute positioned element to its desired independent size
				if(elementAbsolutePositioned(element)){
					if(horizontal){
						element->setSizeInLayout(elementMainSize, elementCrossSize);
					}else {
						element->setSizeInLayout(elementCrossSize, elementMainSize);
					}

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
			Element* element = flexItems.at(i).at(j);
			if(horizontal){
				setWidthInLayout(element, flexItemsBasis.at(i).at(j)*partsize);
			}else{
				setHeightInLayout(element, flexItemsBasis.at(i).at(j)*partsize);
			}
		}
	}

	//set cross size of items if they stretch
	//align items

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
		if(horizontal){
			_parent->setHeightInLayout(newCrossAxisSize);
		}else {
			_parent->setWidthInLayout(newCrossAxisSize);
		}
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

	float currentMainPos = 0;
	float currentCrossPos = spacingCrossAxisStart;

	for(unsigned int i = 0; i < lines.size(); i++){

		//take care of empty space on main axis
		int spacingMainAxisStart = 0;
		int spacingMainAxisBetween = 0;
		if(totalSpaceMainAxis.at(i) > 0){
			switch(getJustifyContent(_parent)){
				case JustifyContent::CENTER:
					spacingMainAxisStart = totalSpaceMainAxis.at(i)/2;
					break;
				case JustifyContent::FLEX_END:
					spacingMainAxisStart = totalSpaceMainAxis.at(i);
					break;
				case JustifyContent::SPACE_AROUND:
					spacingMainAxisStart = totalSpaceMainAxis.at(i)/(lines.at(i).size()+1);
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

			Element* element = lines.at(i).at(j);
			AlignSelf alignSelf = getAlignSelf(element);
			if(alignSelf == AlignSelf::STRETCH ||
			  ((alignSelf == AlignSelf::AUTO) && (alignItems == AlignItems::STRETCH))){
				if(horizontal){
					setHeightInLayout(element, lineSizes.at(i));
				}else{
					setWidthInLayout(element, lineSizes.at(i));
				}
			}else {
				if(horizontal){
					setHeightInLayout(element, getDesiredHeight(element));
				}else{
					setWidthInLayout(element, getDesiredWidth(element));
				}
			}

			//align item

			float elementMainPos = currentMainPos;
			float elementCrossPos = currentCrossPos;
			float elementMainSize = horizontal ? getCurrentWidth(element) : getCurrentHeight(element);
			float elementCrossSize = horizontal ? getCurrentHeight(element) : getCurrentWidth(element);

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

			if(horizontal){
				setPosition(element, ofPoint(elementMainPos, elementCrossPos));
			}else{
				setPosition(element, ofPoint(elementCrossPos, elementMainPos));
			}

			currentMainPos += elementMainSize + spacingMainAxisBetween;

		}

		currentCrossPos += lineSizes.at(i) + spacingCrossAxisBetween;

	}

}

bool FlexBoxLayout::elementFlexing(Element* e){
	if(e->isHidden()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<LayoutPosition>("position") == LayoutPosition::ABSOLUTE){
			return false;
		}
	}
	return true;
}

bool FlexBoxLayout::elementAbsolutePositioned(Element* e){
	if(e->isHidden()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<LayoutPosition>("position") == LayoutPosition::ABSOLUTE){
			return true;
		}
	}
	return false;
}



float FlexBoxLayout::getCurrentWidth(Element* e){
	float res = e->getWidth();
	if(e->hasAttribute("_margin-left")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-left"));
	}
	if(e->hasAttribute("_margin-right")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-right"));
	}
	return res;
}

float FlexBoxLayout::getDesiredWidth(Element* e){
	float res = e->getMinWidth();

	if(e->hasAttribute("_width")){
		std::string widthstr = e->getAttribute<std::string>("_width");
		if(ofIsStringInString(widthstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(widthstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				if(e->parent()){
					return e->parent()->getWidth()*amount;
				}else {
					return ofGetWindowWidth()*amount;
				}
			}
		}else {
			return ofToFloat(widthstr);
		}
	}

	if(e->hasAttribute("_margin-left")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-left"));
	}
	if(e->hasAttribute("_margin-right")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-right"));
	}
	return res;
}

void FlexBoxLayout::setWidthInLayout(Element* e, float width){
	if(e->hasAttribute("_margin-left")){
		width -= ofToFloat(e->getAttribute<std::string>("_margin-left"));
	}
	if(e->hasAttribute("_margin-right")){
		width -= ofToFloat(e->getAttribute<std::string>("_margin-right"));
	}
	e->setWidthInLayout(width);
}

float FlexBoxLayout::getCurrentHeight(Element* e){
	float res = e->getHeight();
	if(e->hasAttribute("_margin-top")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-top"));
	}
	if(e->hasAttribute("_margin-bottom")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-bottom"));
	}
	return res;
}

float FlexBoxLayout::getDesiredHeight(Element* e){
	float res = e->getMinHeight();

	if(e->hasAttribute("_height")){
		std::string heightstr = e->getAttribute<std::string>("_height");
		if(ofIsStringInString(heightstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(heightstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				if(e->parent()){
					return e->parent()->getHeight()*amount;
				}else {
					return ofGetWindowHeight()*amount;
				}
			}
		}else {
			return ofToFloat(heightstr);
		}
	}

	if(e->hasAttribute("_margin-top")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-top"));
	}
	if(e->hasAttribute("_margin-bottom")){
		res += ofToFloat(e->getAttribute<std::string>("_margin-bottom"));
	}
	return res;
}

void FlexBoxLayout::setHeightInLayout(Element* e, float height){
	if(e->hasAttribute("_margin-top")){
		height -= ofToFloat(e->getAttribute<std::string>("_margin-top"));
	}
	if(e->hasAttribute("_margin-bottom")){
		height -= ofToFloat(e->getAttribute<std::string>("_margin-bottom"));
	}
	e->setHeightInLayout(height);
}

void FlexBoxLayout::setPosition(Element* e, ofPoint p){
	if(e->hasAttribute("_margin-left")){
		p.x += ofToFloat(e->getAttribute<std::string>("_margin-left"));
	}
	if(e->hasAttribute("_margin-top")){
		p.y += ofToFloat(e->getAttribute<std::string>("_margin-top"));
	}
	e->setPosition(p);
}

FlexDirection FlexBoxLayout::getFlexDirection(Element *e, FlexDirection defaultVal){
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

JustifyContent FlexBoxLayout::getJustifyContent(Element *e, JustifyContent defaultVal){
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

AlignItems FlexBoxLayout::getAlignItems(Element *e, AlignItems defaultVal){
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

AlignContent FlexBoxLayout::getAlignContent(Element *e, AlignContent defaultVal){
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

AlignSelf FlexBoxLayout::getAlignSelf(Element *e, AlignSelf defaultVal){
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

}} // namespace ofx::DOM
