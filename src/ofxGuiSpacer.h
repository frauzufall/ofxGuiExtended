#pragma once

#include "ofxGuiGroupExtended.h"

class ofxGuiSpacer : public ofxGuiGroupExtended {
public:
    ofxGuiSpacer(std::string collectionName, float size = 10, float x = 10, float y = 10);
    ofxGuiSpacer(float size = 10, float x = 10, float y = 10);
    virtual ~ofxGuiSpacer() {}
    virtual ofxGuiSpacer * setup(float size, float x, float y);
    virtual ofxGuiSpacer * setup(string collectionName, float size, float x, float y);

protected:
    virtual void render();

    virtual void generateDraw();

    void sizeChangedCB();

    ofPath bg;
    float spacing_size;

};
