#pragma once

#include "ofParameter.h"
#include "ofxToggle.h"

class ofxMinimalToggle : public ofxToggle {
public:
    ofxMinimalToggle(ofParameter<bool> _bVal, const ofxToggle::Config & config);
    virtual ~ofxMinimalToggle(){}

protected:
    virtual void render();
    void generateDraw();

    bool setValue(float mx, float my, bool bCheck);
    float getTextWidth(std::string text, float height);

    ofPath border;
};

