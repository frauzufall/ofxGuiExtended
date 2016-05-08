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

		float totalWidth = 0;
		float totalHeight = 0;
		float currentX = DOMLH::getPaddingLeft(_parent);
		float currentY = DOMLH::getPaddingTop(_parent);
		bool resizeParent = false;


		for (DOM::Element* element : children())
		{
			if (element)
			{
				if(!element->isHidden()){

					float w = max(element->getWidth(), DOMLH::getDesiredWidth(element)-DOMLH::getMarginHorizontal(element));
					float h = max(element->getHeight(), DOMLH::getDesiredHeight(element)-DOMLH::getMarginVertical(element));

					element->setSizeInLayout(w,h);

					if(!DOMLH::elementAbsolutePositioned(element)){
						element->setPosition(currentX+DOMLH::getMarginLeft(element), currentY+DOMLH::getMarginTop(element));

						if (getDirection(_parent) == Direction::HORIZONTAL)
						{
							totalHeight = std::max(totalHeight, h+DOMLH::getMarginVertical(element));
							currentX += w+DOMLH::getMarginHorizontal(element);
							totalWidth = currentX;
						}
						else
						{
							totalWidth = std::max(totalWidth, w+DOMLH::getMarginHorizontal(element));
							currentY += h+DOMLH::getMarginVertical(element);
							totalHeight = currentY;
						}
					}else{
//							element->setSizeInLayout(w-getMarginHorizontal(element), h-getMarginVertical(element));
					}

				}
			}
		}

		if (getDirection(_parent) == Direction::HORIZONTAL){
			totalHeight += DOMLH::getPaddingVertical(_parent);
			totalWidth += DOMLH::getPaddingRight(_parent);
		}else{
			totalWidth += DOMLH::getPaddingHorizontal(_parent);
			totalHeight += DOMLH::getPaddingBottom(_parent);
		}

		totalWidth = std::max(totalWidth, DOMLH::getDesiredWidth(_parent)-DOMLH::getMarginHorizontal(_parent));
		totalHeight = std::max(totalHeight, DOMLH::getDesiredHeight(_parent)-DOMLH::getMarginVertical(_parent));

		for (DOM::Element* element : children())
		{
			if (element)
			{
				if(!element->isHidden()){

					if(!DOMLH::elementAbsolutePositioned(element)){
						if (getDirection(_parent) == Direction::HORIZONTAL){
							element->setHeightInLayout(totalHeight-DOMLH::getPaddingVertical(_parent)-DOMLH::getMarginVertical(element));
						}else{
							element->setWidthInLayout(totalWidth-DOMLH::getPaddingHorizontal(_parent)-DOMLH::getMarginHorizontal(element));
						}
					}
				}
			}
		}

		if(totalWidth != _parent->getWidth() || totalHeight != _parent->getHeight()){
			resizeParent = true;
		}

		if(totalWidth != _parent->getWidth() || totalHeight != _parent->getHeight()){
			_parent->setSizeInLayout(totalWidth, totalHeight);
			_parent->invalidateChildShape();
		}

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

