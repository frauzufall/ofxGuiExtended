// =============================================================================
//
// Copyright (c) 2009-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofMain.h"
#include "ofxDOMBoxLayout.h"
#include "../DOM/Element.h"
#include "../ofxGuiElement.h"
#include <iostream>
#include "ofxDOMLayoutHelper.h"

typedef ofxDOMLayoutHelper DOMLH;


ofxDOMBoxLayout::ofxDOMBoxLayout(DOM::Element* parent):
	DOM::_Layout<ofxDOMBoxLayout>(parent)
{
}


ofxDOMBoxLayout::~ofxDOMBoxLayout()
{
}


void ofxDOMBoxLayout::doLayout()
{
	if (_parent && !_isDoingLayout)
	{

		// Prevent recursive calls to doLayout.
		_isDoingLayout = true;

		bool horizontal = getDirection(_parent) == Direction::HORIZONTAL;


//		if(ofxGuiElement* el = dynamic_cast<ofxGuiElement*>(_parent)){
//			cout << el->getName() << endl;
//		}

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

		float totalWidth = DOMLH::getDesiredWidth(_parent, wParent) - paddingHorizontal;
		float totalHeight = DOMLH::getDesiredHeight(_parent, hParent) - paddingVertical;

		float maxWidth = DOMLH::getMaxWidth(_parent, wParent) - paddingHorizontal;
		float maxHeight = DOMLH::getMaxHeight(_parent, hParent) - paddingVertical;

		float currentX = DOMLH::getPaddingLeft(_parent);
		float currentY = DOMLH::getPaddingTop(_parent);

		for (DOM::Element* element : children()){
			if (element){
				if(element->getVisible().get()){

					// set to minimal size on main axis
					if(horizontal){
						element->setSizeByParent(0, DOMLH::getMaxHeight(element, maxHeight) + DOMLH::getMarginVertical(element));
					}else{
						element->setSizeByParent(DOMLH::getMaxWidth(element, maxWidth) + DOMLH::getMarginHorizontal(element),0);
					}


					float w = DOMLH::getDesiredWidth(element, maxWidth);
					float h = DOMLH::getDesiredHeight(element, maxHeight);

					if(!DOMLH::elementAbsolutePositioned(element)){

						element->setPosition(currentX+DOMLH::getMarginLeft(element), currentY+DOMLH::getMarginTop(element));

						element->setLayoutSize(w, h, true);
						w = element->getWidth();
						h = element->getHeight();

						if(horizontal){
							totalHeight = std::max(totalHeight, h+DOMLH::getMarginVertical(element));
							currentX += w + DOMLH::getMarginHorizontal(element);
//							totalWidth = currentX;
						}
						else{
							totalWidth = std::max(totalWidth, w + DOMLH::getMarginHorizontal(element));
							currentY += h + DOMLH::getMarginVertical(element);
//							totalHeight = currentY;
						}
					}else{
						element->setLayoutSize(w, h);
					}

				}
			}
		}

		if(horizontal){
			totalWidth = max(currentX+DOMLH::getPaddingRight(_parent), DOMLH::getDesiredWidth(_parent, wParent));
		}else{
			totalHeight = max(currentY+DOMLH::getPaddingBottom(_parent), DOMLH::getDesiredHeight(_parent, hParent));
		}

		if(!DOMLH::elementAbsolutePositioned(_parent)){
			if(horizontal){
				totalHeight = max(totalHeight, maxHeight);
			}else{
				totalWidth = max(totalWidth, maxWidth);
			}
		}

		// set cross axis size of all children to maximum
		for (DOM::Element* element : children())
		{
			if (element)
			{
				if(element->getVisible().get()){

					if(!DOMLH::elementAbsolutePositioned(element)){

						if (horizontal){
							float h = totalHeight-DOMLH::getMarginVertical(element);
							element->setLayoutHeight(h, true);
						}else{
							float w = totalWidth-DOMLH::getMarginHorizontal(element);
							element->setLayoutWidth(w, true);
						}

					}
				}
			}
		}

		if(horizontal){
			totalHeight += paddingVertical;
		}else{
			totalWidth += paddingHorizontal;
		}

		_parent->setLayoutSize(totalWidth, totalHeight, false);

		_isDoingLayout = false;
	}
}

ofxDOMBoxLayout::Direction ofxDOMBoxLayout::getDirection(DOM::Element *e, Direction defaultVal){
	if(e->hasAttribute("_direction")){
		std::string val = e->getAttribute<std::string>("_direction");
		if(val == "horizontal"){
			return Direction::HORIZONTAL;
		}
		if(val == "vertical"){
			return Direction::VERTICAL;
		}
	}
	return defaultVal;
}

