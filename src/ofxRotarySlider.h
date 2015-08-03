#pragma once

#include "ofxSlider.h"
#include "ofParameter.h"

template<typename Type>
class ofxRotarySlider : public ofxSlider<Type>{
public:
	struct Config: public ofxSlider<Type>::Config{
		Config(){}
		Config(const typename ofxSlider<Type>::Config & config)
		:ofxSlider<Type>::Config(config){}
		Config(const ofxBaseGui::Config & config)
		:ofxSlider<Type>::Config(config){}

	};
	ofxRotarySlider(ofParameter<Type> _val, const Config & config);
    virtual ~ofxRotarySlider();
    virtual bool mousePressed(ofMouseEventArgs & args);

protected:
    virtual void render();
    bool setValue(float mx, float my, bool bCheck);
    void generateDraw();
    void generateText();
    void valueChanged(Type & value);
    void arcStrip(ofPath& path, ofPoint center, float outer_radius, float inner_radius, float percent);
    float _mouseOffset;
};

typedef ofxRotarySlider<float> ofxFloatRotarySlider;
typedef ofxRotarySlider<int> ofxIntRotarySlider;
