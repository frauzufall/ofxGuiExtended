#pragma once

#include "ofxBaseGui.h"

class ofxGuiSpacer : public ofxBaseGui {
public:
    ofxGuiSpacer(std::string name, float size = 10, float x = 10, float y = 10);
    ofxGuiSpacer(float size = 10, float x = 10, float y = 10);
    virtual ~ofxGuiSpacer() {}
    virtual ofxGuiSpacer * setup(float size, float x, float y);
    virtual ofxGuiSpacer * setup(string name, float size, float x, float y);

    virtual ofAbstractParameter & getParameter();

    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
    virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

protected:
    virtual void render();
    virtual bool setValue(float mx, float my, bool bCheckBounds){return false;}
    virtual void generateDraw();

    void sizeChangedCB();

    ofPath bg;
    float spacing_size;

    // not needed, but has be be there
    ofParameter<bool> parameter;

};
