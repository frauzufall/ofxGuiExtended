#pragma once

#include "ofxSlider.h"

template<typename Type>
class ofxVerticalSlider : public ofxSlider<Type>{
	friend class ofPanel;
	
public:	
    ofxVerticalSlider();
    ~ofxVerticalSlider();
    ofxVerticalSlider(ofParameter<Type> _val, float width = ofxBaseGui::defaultHeight, float height = ofxBaseGui::defaultWidth);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
        this->value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
        this->value.removeListener(listener,method);
	}

protected:
	bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
    virtual void generateText();
    virtual void render();
};

typedef ofxVerticalSlider<float> ofxFloatVerticalSlider;
typedef ofxVerticalSlider<int> ofxIntVerticalSlider;
