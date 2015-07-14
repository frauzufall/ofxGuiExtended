#include "ofxZoomableCanvas.h"
#include "ofGraphics.h"
using namespace std;

ofxZoomableCanvas::ofxZoomableCanvas() {}

ofxZoomableCanvas::ofxZoomableCanvas(string canvasName, ofTexture *tex, float w, float h){
    setup(canvasName,tex,w,h);
}

ofxZoomableCanvas::ofxZoomableCanvas(ofTexture *tex, float w, float h){
    setup("",tex,w,h);
}

ofxZoomableCanvas::~ofxZoomableCanvas(){
}

ofxZoomableCanvas* ofxZoomableCanvas::setup(string canvasName, ofTexture *tex, float w, float h) {
    ofxCanvas::setup(canvasName, tex, w, h);
    zoom_factor = 0;
    zoom_speed = 0.1;
    dragging_dst = false;
    setNeedsRedraw();
    return this;
}

void ofxZoomableCanvas::setSize(float w, float h){
    ofxCanvas::setSize(w,h);
    contentFbo.clear();
    contentFbo.allocate(b.width, b.height, GL_RGBA);
}

void ofxZoomableCanvas::generateDraw(){
    ofxCanvas::generateDraw();

    if(texture) {
        if(texture->isAllocated()) {

            contentFbo.begin();
            ofClear(0,0,0,0);

            ofPushMatrix();

            ofTranslate(-b.getPosition());

            zoom_translation = zoom_point-zoom_point_scaled+zoom_point_offset;
            if(zoom_translation.x > 0) zoom_translation.x = 0;
            if(zoom_translation.y > 0) zoom_translation.y = 0;
            if(zoom_translation.x < -addZoom(b.getWidth())+b.getWidth())
                zoom_translation.x = -addZoom(b.getWidth())+b.getWidth();
            if(zoom_translation.y < -addZoom(b.getHeight())+b.getHeight())
                zoom_translation.y = -addZoom(b.getHeight())+b.getHeight();
            ofTranslate(zoom_translation);

            texture->draw(b.getPosition(), addZoom(b.getWidth()), addZoom(b.getHeight()));

            ofPopMatrix();

            contentFbo.end();
        }
    }

}

void ofxZoomableCanvas::render() {
    ofColor c = ofGetStyle().color;

    bg.draw();
    if(texture) {
        if(texture->isAllocated()) {
            contentFbo.draw(b);
        }
    }


    ofBlendMode blendMode = ofGetStyle().blendingMode;
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableAlphaBlending();
    }
    ofSetColor(textColor);

    bindFontTexture();
    textMesh.draw();
    unbindFontTexture();

    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}

ofAbstractParameter & ofxZoomableCanvas::getParameter(){
    return label;
}

bool ofxZoomableCanvas::mouseDragged(ofMouseEventArgs &args) {
    ofPoint mouse(args.x, args.y);
    if(dragging_dst) {
        zoom_point_offset += mouse - last_mouse;
        last_mouse = mouse;
        setNeedsRedraw();
    }
    return false;
}

bool ofxZoomableCanvas::mousePressed(ofMouseEventArgs &args) {
    ofPoint mouse(args.x, args.y);
    if(b.inside(mouse)) {
        dragging_dst = true;
        last_mouse = mouse;
    }
    return false;
}

bool ofxZoomableCanvas::mouseReleased(ofMouseEventArgs &args) {
    dragging_dst = false;
    return false;
}

bool ofxZoomableCanvas::mouseScrolled(ofMouseEventArgs &args) {

    if(b.inside(ofGetMouseX(), ofGetMouseY())) {
        setZoomFactor(args.y);
        setNeedsRedraw();
    }

    return false;
}

void ofxZoomableCanvas::setZoomFactor(int factor) {

    int old_zoom_factor = zoom_factor;

    zoom_factor += factor;
    if(zoom_factor < 0)
        zoom_factor = 0;

    ofPoint zoom_point_old = zoom_point;

    ofPoint tmp_zoom_point;
    tmp_zoom_point.x = ofGetMouseX() - b.x - zoom_point_offset.x;
    tmp_zoom_point.y = ofGetMouseY() - b.y - zoom_point_offset.y;

    ofVec2f diff = tmp_zoom_point - zoom_point_old;

    if(old_zoom_factor == 0) {
        diff = ofPoint(0,0);
        zoom_point_offset = ofPoint(0,0);
        zoom_point_old = tmp_zoom_point;
    }

    zoom_point = zoom_point_old + removeZoom(diff);
    zoom_point_offset += tmp_zoom_point - zoom_point;
    zoom_point_scaled = addZoom(zoom_point);

}

ofPoint ofxZoomableCanvas::addZoom(ofPoint p) {
    return p*(1+zoom_factor*zoom_speed);
}

float ofxZoomableCanvas::addZoom(float p) {
    return p*(1+zoom_factor*zoom_speed);
}


ofPoint ofxZoomableCanvas::removeZoom(ofPoint p) {
    return p/(1+zoom_factor*zoom_speed);
}

