#pragma once
#include "ofxPanelExtended.h"

class ofxGuiPage : public ofxPanelExtended {

public:

    ofxGuiPage();
    virtual ~ofxGuiPage() {}
    virtual ofxGuiPage * setup(std::string pageName="", std::string filename="settings.xml", float x = 10, float y = 10);

    void add(ofxBaseGui * element);

    void clear();

    virtual bool mouseDragged(ofMouseEventArgs &args);

    virtual void setWidthElements(float w);
    virtual void scaleWidthElements(float factor);

protected:
    virtual void sizeChangedCB();

};
