#pragma once

#include "ofxCanvas.h"
#include "ofFbo.h"

class ofxZoomableCanvas: public ofxCanvas {
public:
    ofxZoomableCanvas();
    ofxZoomableCanvas(string canvasName, ofTexture* tex = 0, float w = 0, float h = 0);
    ofxZoomableCanvas(ofTexture* tex, float w = defaultWidth, float h = 0);
    virtual ~ofxZoomableCanvas();

    ofxZoomableCanvas * setup(string canvasName = "", ofTexture* tex = 0, float w = 0, float h = 0);

    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args);
    virtual bool mouseDragged(ofMouseEventArgs & args);
    virtual bool mouseReleased(ofMouseEventArgs & args);
    virtual bool mouseScrolled(ofMouseEventArgs & args);

    virtual void setSize(float w, float h);

    virtual ofAbstractParameter & getParameter();

protected:

    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    void update();
    void render();
    void generateDraw();

    void setZoomFactor(int factor);
    ofPoint addZoom(ofPoint p);
    float addZoom(float p);
    ofPoint removeZoom(ofPoint p);

    int zoom_factor;
    float zoom_speed;
    ofPoint zoom_point, zoom_point_scaled, zoom_point_offset;
    ofPoint zoom_translation;

    bool dragging_dst;
    ofPoint last_mouse;

    ofFbo contentFbo;

};
