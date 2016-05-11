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

		float paddingHorizontal = DOMLH::getPaddingHorizontal(_parent);
		float paddingVertical = DOMLH::getPaddingVertical(_parent);
		float marginHorizontal = DOMLH::getMarginHorizontal(_parent);
		float marginVertical = DOMLH::getMarginVertical(_parent);

		float totalWidth = std::max(_parent->getSizeByParent().x, DOMLH::getDesiredWidth(_parent));
		float totalHeight = std::max(_parent->getSizeByParent().y, DOMLH::getDesiredHeight(_parent));

		totalWidth -= marginHorizontal;
		totalHeight -= marginVertical;

		totalWidth -= paddingHorizontal;
		totalHeight -= paddingVertical;

		float currentX = DOMLH::getPaddingLeft(_parent);
		float currentY = DOMLH::getPaddingTop(_parent);

		for (DOM::Element* element : children()){
			if (element){
				if(!element->isHidden()){

					float w = DOMLH::getDesiredWidth(element, totalWidth)-DOMLH::getMarginHorizontal(element);
					float h = DOMLH::getDesiredHeight(element, totalHeight)-DOMLH::getMarginVertical(element);

					if(!DOMLH::elementAbsolutePositioned(element)){
						element->setPosition(currentX+DOMLH::getMarginLeft(element), currentY+DOMLH::getMarginTop(element));

						if (getDirection(_parent) == Direction::HORIZONTAL)
						{
							w = max(element->getWidth(), w);
							element->setLayoutWidth(w, false);
							totalHeight = std::max(totalHeight, h);
							currentX += w + DOMLH::getMarginHorizontal(element);
							totalWidth = currentX;
						}
						else
						{
							h = max(element->getHeight(), h);
							element->setLayoutHeight(h, false);
							totalWidth = std::max(totalWidth, w);
							currentY += h + DOMLH::getMarginVertical(element);
							totalHeight = currentY;
						}
					}/*else{
							element->setSizeInLayout(w-DOMLH::getMarginHorizontal(element), h-DOMLH::getMarginVertical(element));
					}*/

				}
			}
		}

		if (getDirection(_parent) == Direction::HORIZONTAL){
			totalWidth += DOMLH::getPaddingRight(_parent);
		}else{
			totalHeight += DOMLH::getPaddingBottom(_parent);
		}

		// set cross axis size of all children to maximum
		for (DOM::Element* element : children())
		{
			if (element)
			{
				if(!element->isHidden()){

					if(!DOMLH::elementAbsolutePositioned(element)){

						if (getDirection(_parent) == Direction::HORIZONTAL){
							float h = totalHeight-DOMLH::getMarginVertical(element);
							element->setLayoutHeight(h, false);
							element->setSizeByParent(0, totalHeight);
						}else{
							float w = totalWidth-DOMLH::getMarginHorizontal(element);
							element->setLayoutWidth(w, false);
							element->setSizeByParent(totalWidth, 0);
						}

					}
				}
			}
		}

		if (getDirection(_parent) == Direction::HORIZONTAL){
			totalHeight += paddingVertical;
		}else{
			totalWidth += paddingHorizontal;
		}

		_parent->setLayoutSize(totalWidth, totalHeight);

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

