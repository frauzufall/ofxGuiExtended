#pragma once

#include "view/ofxDOMBoxLayout.h"
#include "ofParameter.h"
#include "Element.h"

class ofxDOMFloatingBoxLayout: public ofxDOMBoxLayout
{
public:
	/// If the Orientation::DEFAULT is chosen, the default will be set to
	/// Orientation::HORIZONTAL.
	ofxDOMFloatingBoxLayout(DOM::Element* parent, DOM::Orientation orientation = DOM::Orientation::VERTICAL);

	virtual ~ofxDOMFloatingBoxLayout();

	virtual void doLayout() override;

protected:

	bool elementFloating(DOM::Element* e);

	void setNextLeftFloatPosition(ofPoint p, DOM::Element* e);
	void setNextRightFloatPosition(ofPoint p, DOM::Element* e);
	ofPoint nextLeftFloatPositionLoop(DOM::Element* e, DOM::Element *s, const ofPoint& res);
	ofPoint nextRightFloatPositionLoop(DOM::Element* e, DOM::Element *s, const ofPoint& res);

	float getRight(DOM::Element* e);
	float getLeft(DOM::Element* e);
	float getTop(DOM::Element* e);
	float getBottom(DOM::Element* e);
	float getWidth(DOM::Element* e);
	float getHeight(DOM::Element* e);

	void setPosition(DOM::Element* e, ofPoint p);

};
