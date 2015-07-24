#pragma once
#include "ofxGuiGroupExtended.h"

class ofxGuiMatrix : public ofxGuiGroupExtended {
public:
    ofxGuiMatrix();
    ofxGuiMatrix(std::string collectionName, int cols=0, std::string _filename="settings.xml", float x = 10, float y = 10);
    ofxGuiMatrix(const ofParameterGroup & parameters, int cols=0, std::string _filename="settings.xml", float x = 10, float y = 10);
    virtual ~ofxGuiMatrix() {}
    virtual ofxGuiMatrix * setup(std::string collectionName="", int cols=0, std::string filename="settings.xml", float x = 10, float y = 10);
    virtual ofxGuiMatrix * setup(const ofParameterGroup & parameters, int cols=0, std::string filename="settings.xml", float x = 10, float y = 10);

    void add(ofxBaseGui * element);

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

    void setColNum(int num);
    int getColNum();
    int getRowNum();

    float getElementWidth();
    float getElementHeight();

    void setElementHeight(float h);

protected:
    void updateElementWidth();
    virtual void sizeChangedCB();

    virtual bool setValue(float mx, float my, bool bCheck);

    int numCol=0;
    float w_matrix=0, w_element=0, h_element=20;

};
