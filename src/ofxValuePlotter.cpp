#include "ofxValuePlotter.h"
#include "ofGraphics.h"
using namespace std;

ofxValuePlotter::ofxValuePlotter(ofParameter<float> value, const Config & config)
:ofxBaseGui(config)
,plotSize(config.plotSize)
,minVal(config.minValue)
,maxVal(config.maxValue)
,decimalPlace(config.decimalPlace){
    autoscale = minVal == maxVal;
    buffer.clear();
	this->value.makeReferenceTo(value);
    this->value.addListener(this, &ofxValuePlotter::update);
    setNeedsRedraw();
}

ofxValuePlotter::~ofxValuePlotter(){
}

void ofxValuePlotter::update(float & value) {
    if(plotSize > 0) {
        buffer.push_back(value);

        if( (int)buffer.size() > plotSize ) {
            buffer.erase(buffer.begin(), buffer.begin()+1);
        }
        if(autoscale) {
            if(value < minVal) minVal = value;
            if(value > maxVal) maxVal = value;
        }
    }

    setNeedsRedraw();

}

void ofxValuePlotter::setDecimalPlace(int place) {
    decimalPlace = place;
}

void ofxValuePlotter::generateDraw(){
    bg.clear();

    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(b);

    string name = ofToString(value,decimalPlace) +  " " + this->getName();

    textMesh = getTextMesh(name, b.x+textPadding, b.y + b.height / 2 + 4);

    if(plotSize > 0) {
        plot.clear();
        if(minVal != maxVal && buffer.size() > 1) {
            plot.moveTo(b.x, b.y + b.height);
            unsigned int i;
            for(i = 0; i < buffer.size(); i++) {
                float x = ofMap(i, 0, buffer.size()-1, b.x, b.x+b.width);
                float y = ofMap(buffer[i], minVal, maxVal, b.y+b.height, b.y);
                plot.lineTo(x,y);
            }
            plot.lineTo(b.x + b.width, b.y + b.height);
            plot.close();
            plot.setFilled(true);
            plot.setFillColor(thisFillColor);
        }
    }
}

void ofxValuePlotter::render() {
    ofColor c = ofGetStyle().color;

    bg.draw();
    if(plotSize > 0) {
        plot.draw();
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

ofAbstractParameter & ofxValuePlotter::getParameter(){
    return value;
}
