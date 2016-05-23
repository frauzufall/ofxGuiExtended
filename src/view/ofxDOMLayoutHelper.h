#pragma once

#include "../DOM/Element.h"

class ofxDOMLayoutHelper
{
	public:
		ofxDOMLayoutHelper();

		static bool elementAbsolutePositioned(DOM::Element* e);

		static float getDesiredWidth(DOM::Element* e);
		static float getDesiredHeight(DOM::Element* e);
		static float getDesiredWidth(DOM::Element* e, float parentWidth);
		static float getDesiredHeight(DOM::Element* e, float parentHeight);
		static float getDesiredWidthStretched(DOM::Element* e, float parentWidth);
		static float getDesiredHeightStretched(DOM::Element* e, float parentHeight);
		static float getMaxWidth(DOM::Element* e, float parentWidth);
		static float getMaxHeight(DOM::Element* e, float parentHeight);

		static float getMarginHorizontal(DOM::Element* e);
		static float getMarginVertical(DOM::Element* e);
		static float getMarginLeft(DOM::Element* e);
		static float getMarginRight(DOM::Element* e);
		static float getMarginTop(DOM::Element* e);
		static float getMarginBottom(DOM::Element* e);

		static float getPaddingHorizontal(DOM::Element* e);
		static float getPaddingVertical(DOM::Element* e);
		static float getPaddingLeft(DOM::Element* e);
		static float getPaddingRight(DOM::Element* e);
		static float getPaddingTop(DOM::Element* e);
		static float getPaddingBottom(DOM::Element* e);

		static void setPosition(DOM::Element* e, ofPoint p);
};
