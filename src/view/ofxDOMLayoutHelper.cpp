#include "ofxDOMLayoutHelper.h"
#include "JsonConfigParser.h"
#include "../DOM/Layout.h"
#include "../ofxGuiElement.h"

ofxDOMLayoutHelper::ofxDOMLayoutHelper()
{

}

bool ofxDOMLayoutHelper::elementAbsolutePositioned(DOM::Element* e){
	if(!e->getVisible().get()){
		return false;
	}
	if(e->hasAttribute("position")){
		if(e->getAttribute<DOM::LayoutPosition>("position") == DOM::LayoutPosition::POSITION_ABSOLUTE){
			return true;
		}
	}
	return false;
}

float ofxDOMLayoutHelper::getDesiredWidth(DOM::Element* e){
	if(e->parent()){
		return getDesiredWidth(e, e->parent()->getWidth()-getPaddingHorizontal(e->parent()));
	}else{
		return getDesiredWidth(e, ofGetWindowWidth());
	}

}

float ofxDOMLayoutHelper::getDesiredWidth(DOM::Element* e, float parentWidth){
	float res = e->getMinWidth() + getPaddingHorizontal(e);

	if(e->hasAttribute("_width")){
		std::string widthstr = e->getAttribute<std::string>("_width");
		if(ofIsStringInString(widthstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(widthstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				return parentWidth*amount-getMarginHorizontal(e);
			}
		}else {
			float val = ofToFloat(widthstr);
			if(val > 0){
				return val;
			}
		}
	}

	return res;
}

float ofxDOMLayoutHelper::getDesiredWidthStretched(DOM::Element* e, float parentWidth){
	float res = e->getMinWidth() + getPaddingHorizontal(e);

	if(e->hasAttribute("_width")){
		std::string widthstr = e->getAttribute<std::string>("_width");
		if(ofIsStringInString(widthstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(widthstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				return parentWidth*amount-getMarginHorizontal(e);
			}
		}else {
			float val = ofToFloat(widthstr);
			if(val > 0){
				return val;
			}
		}
	}

	return max(res, parentWidth-getMarginHorizontal(e));
}

float ofxDOMLayoutHelper::getMaxWidth(DOM::Element* e, float parentWidth){

	if(e->hasAttribute("_width")){
		std::string widthstr = e->getAttribute<std::string>("_width");
		if(ofIsStringInString(widthstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(widthstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				return parentWidth*amount-getMarginHorizontal(e);
			}
		}else {
			float val = ofToFloat(widthstr);
			if(val > 0){
				return val;
			}
		}
	}

	return parentWidth-getMarginHorizontal(e);
}


float ofxDOMLayoutHelper::getDesiredHeight(DOM::Element* e){
	if(e->parent()){
		return getDesiredHeight(e, e->parent()->getHeight()-getPaddingVertical(e->parent()));
	}else{
		return getDesiredHeight(e, ofGetWindowHeight());
	}
}

float ofxDOMLayoutHelper::getDesiredHeight(DOM::Element* e, float parentHeight){
	float res = e->getMinHeight() + getPaddingVertical(e);

	if(e->hasAttribute("_height")){
		std::string heightstr = e->getAttribute<std::string>("_height");
		if(ofIsStringInString(heightstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(heightstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				return parentHeight*amount-getMarginVertical(e);
			}
		}else {
			float val = ofToFloat(heightstr);
			if(val > 0){
				return val;
			}
		}
	}

	return res;
}

float ofxDOMLayoutHelper::getDesiredHeightStretched(DOM::Element* e, float parentHeight){
	float res = e->getMinHeight() + getPaddingVertical(e);

	if(e->hasAttribute("_height")){
		std::string heightstr = e->getAttribute<std::string>("_height");
		if(ofIsStringInString(heightstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(heightstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				return parentHeight*amount-getMarginVertical(e);
			}
		}else {
			float val = ofToFloat(heightstr);
			if(val > 0){
				return val;
			}
		}
	}

	return max(res, parentHeight -getMarginVertical(e));
}

float ofxDOMLayoutHelper::getMaxHeight(DOM::Element* e, float parentHeight){

	if(e->hasAttribute("_height")){
		std::string heightstr = e->getAttribute<std::string>("_height");
		if(ofIsStringInString(heightstr, "%")){
			vector<std::string> _val = JsonConfigParser::getMatchedStrings(heightstr, "(?:\\b|-)([1-9]{1,2}[0]?|100)\\b");
			if(_val.size() > 0){
				float amount = ofToFloat(_val[0])/100.;
				return parentHeight*amount-getMarginVertical(e);
			}
		}else {
			float val = ofToFloat(heightstr);
			if(val > 0){
				return val;
			}
		}
	}

	return parentHeight -getMarginVertical(e);
}

float ofxDOMLayoutHelper::getMarginHorizontal(DOM::Element *e){
	return getMarginLeft(e) + getMarginRight(e);
}

float ofxDOMLayoutHelper::getMarginVertical(DOM::Element *e){
	return getMarginTop(e) + getMarginBottom(e);
}

float ofxDOMLayoutHelper::getMarginLeft(DOM::Element *e){
	if(e->hasAttribute("_margin-left")){
		return ofToFloat(e->getAttribute<std::string>("_margin-left"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getMarginRight(DOM::Element *e){
	if(e->hasAttribute("_margin-right")){
		return ofToFloat(e->getAttribute<std::string>("_margin-right"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getMarginTop(DOM::Element *e){
	if(e->hasAttribute("_margin-top")){
		return ofToFloat(e->getAttribute<std::string>("_margin-top"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getMarginBottom(DOM::Element *e){
	if(e->hasAttribute("_margin-bottom")){
		return ofToFloat(e->getAttribute<std::string>("_margin-bottom"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getPaddingHorizontal(DOM::Element *e){
	return getPaddingLeft(e) + getPaddingRight(e);
}

float ofxDOMLayoutHelper::getPaddingVertical(DOM::Element *e){
	return getPaddingTop(e) + getPaddingBottom(e);
}

float ofxDOMLayoutHelper::getPaddingLeft(DOM::Element *e){
	if(e->hasAttribute("_padding-left")){
		return ofToFloat(e->getAttribute<std::string>("_padding-left"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getPaddingRight(DOM::Element *e){
	if(e->hasAttribute("_padding-right")){
		return ofToFloat(e->getAttribute<std::string>("_padding-right"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getPaddingTop(DOM::Element *e){
	if(e->hasAttribute("_padding-top")){
		return ofToFloat(e->getAttribute<std::string>("_padding-top"));
	}
	return 0;
}

float ofxDOMLayoutHelper::getPaddingBottom(DOM::Element *e){
	if(e->hasAttribute("_padding-bottom")){
		return ofToFloat(e->getAttribute<std::string>("_padding-bottom"));
	}
	return 0;
}

void ofxDOMLayoutHelper::setPosition(DOM::Element* e, ofPoint p){
	if(e->layout()){
		if(e->layout()->isDoingLayout()){
			return;
		}
	}
	p.x += getMarginLeft(e);
	p.y += getMarginTop(e);
	e->setPosition(p);
}
