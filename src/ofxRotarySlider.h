#pragma once

#include "ofxSlider.h"
#include "ofParameter.h"

template<typename Type>
class ofxRotarySlider : public ofxSlider<Type>{
    friend class ofPanel;

public:
    ofxRotarySlider();
    ~ofxRotarySlider();
    ofxRotarySlider(ofParameter<Type> _val, float width = ofxBaseGui::defaultWidth, float height = ofxBaseGui::defaultHeight);

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
