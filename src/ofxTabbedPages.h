#pragma once

#include "ofxGuiPage.h"

class ofxTabbedPages : public ofxGuiPage {

public:
    ofxTabbedPages();
    ~ofxTabbedPages();

    ofxTabbedPages * setup(std::string collectionName="", std::string filename="settings.xml", float x = 10, float y = 10);

    void add(ofxGuiPage * element);

    void clear();

    void setActiveTab(int index);
    int getActiveTabIndex();
    ofxGuiPage* getActiveTab();

    void setTabHeight(int h);
    void setTabWidth(int w);

    bool mouseDragged(ofMouseEventArgs & args);
    bool mouseReleased(ofMouseEventArgs & args);

protected:
    void render();
    bool setValue(float mx, float my, bool bCheck);
    void generateDraw();
    void updateContentShapes();
    void setSizeToElement(ofxBaseGui *element);
    virtual void sizeChangedCB();

private:

    ofxGuiGroupExtended tabs;
    vector<ofParameter<bool>> parameters_tabs;
    float tabHeight, tabWidth;
    ofRectangle pagesShape, tabShape;
    ofPath bg;
    ofPath tabBorder;
    ofxBaseGui* activePage;
    ofxBaseGui* activeToggle;
};
