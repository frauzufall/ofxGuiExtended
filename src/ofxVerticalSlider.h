#pragma once

#include "ofxSlider.h"

template<typename Type>
class ofxVerticalSlider : public ofxSlider<Type>{
public:	
	struct Config: public ofxSlider<Type>::Config{
		Config(){}
		Config(const typename ofxSlider<Type>::Config & config)
		:ofxSlider<Type>::Config(config){}
		Config(const ofxBaseGui::Config & config)
		:ofxSlider<Type>::Config(config){}

	};
    ofxVerticalSlider(ofParameter<Type> _val, const Config & config);
    virtual ~ofxVerticalSlider();

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
