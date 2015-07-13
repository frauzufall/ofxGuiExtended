#pragma once

#include "ofxGuiGroupExtended.h"

class ofxGuiGroup;

class ofxPanelExtended : public ofxGuiGroupExtended {

    friend class ofxGuiPage;
    friend class ofxTabbedPages;

public:
    ofxPanelExtended();
    ofxPanelExtended(const ofParameterGroup & parameters, std::string filename="settings.xml", float x = 10, float y = 10);
    ~ofxPanelExtended();

    ofxPanelExtended * setup(std::string collectionName="", std::string filename="settings.xml", float x = 10, float y = 10);
    ofxPanelExtended * setup(const ofParameterGroup & parameters, std::string filename="settings.xml", float x = 10, float y = 10);

    bool mouseReleased(ofMouseEventArgs & args);

    ofEvent<void> loadPressedE;
    ofEvent<void> savePressedE;
protected:
    void render();
    void renderHeader();
    bool setValue(float mx, float my, bool bCheck);
    void generateDraw();
    void generateHeader();
    void loadIcons();
private:
    ofRectangle loadBox, saveBox;
    static ofImage loadIcon, saveIcon;

    ofPoint grabPt;
    bool bGrabbed;
};
