#pragma once
#include "ofxGuiGroup.h"


class ofxGuiGroupExtended : public ofxGuiGroup {
public:
    ofxGuiGroupExtended();
    ofxGuiGroupExtended(const ofParameterGroup & parameters, std::string _filename="settings.xml", float x = 10, float y = 10);
    virtual ~ofxGuiGroupExtended() {}
    virtual ofxGuiGroupExtended * setup(std::string collectionName="", std::string filename="settings.xml", float x = 10, float y = 10);
    virtual ofxGuiGroupExtended * setup(const ofParameterGroup & parameters, std::string filename="settings.xml", float x = 10, float y = 10);

    void add(ofxBaseGui * element);

    void clear();

    void minimize();
    void maximize();
    void minimizeAll();
    void maximizeAll();

    virtual bool mouseMoved(ofMouseEventArgs & args);
    virtual bool mousePressed(ofMouseEventArgs & args);
    virtual bool mouseDragged(ofMouseEventArgs & args);
    virtual bool mouseReleased(ofMouseEventArgs & args);
    virtual bool mouseScrolled(ofMouseEventArgs & args);

    void setHeightElements(float h);

protected:
    void sizeChangedCB();
    virtual void render();
    virtual void generateDraw();
    bool vertical;

    ofxGuiGroupExtended * parent;

};
