#pragma once
#include "ofxGuiGroup.h"
#include "ofxToggle.h"

class ofxGuiGroupExtended : public ofxGuiGroup {

    friend class ofxGuiMatrix;
    friend class ofxGuiSpacer;
    friend class ofxGuiPage;
    friend class ofxTabbedPages;

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

    virtual void setWidthElements(float w);
    virtual void scaleWidthElements(float factor);

    void setAlignHorizontal();
    void setAlignVertical();

    bool isAlignedVertical();

    void setShowHeader(bool show);

    void allowMultipleActiveToggles(bool allow);
    bool setActiveToggle(int index);
    bool setActiveToggle(ofxToggle* toggle);
    int getActiveToggleIndex();

protected:
    virtual void sizeChangedCB();
    virtual void render();
    virtual void generateDraw();
    bool _bVertical;
    bool _bUseHeader;
    bool _bAllowMultiple;

    virtual bool setValue(float mx, float my, bool bCheck);

    int active_toggle_index;
    bool processToggles(ofxToggle *toggle, ofMouseEventArgs a);
    void setOneToggleActive();
    void deactivateAllOtherToggles(ofxToggle* toggle);

    float getContentHeight();
    void setContentHeight(float h);
    float getContentWidth();
    void setContentWidth(float w);

};
