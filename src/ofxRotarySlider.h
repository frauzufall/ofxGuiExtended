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
    virtual bool mouseReleased(ofMouseEventArgs & args);

    template<class ListenerClass, typename ListenerMethod>
    void addListener(ListenerClass * listener, ListenerMethod method){
        this->value.addListener(listener,method);
    }

    template<class ListenerClass, typename ListenerMethod>
    void removeListener(ListenerClass * listener, ListenerMethod method){
        this->value.removeListener(listener,method);
    }

protected:
    virtual void render();
    bool setValue(float mx, float my, bool bCheck);
    void generateDraw();
    void generateText();
    void valueChanged(Type & value);
    void arcStrip(ofPath& path, ofPoint center, float outer_radius, float inner_radius, float percent);
};

typedef ofxRotarySlider<float> ofxFloatRotarySlider;
typedef ofxRotarySlider<int> ofxIntRotarySlider;
