#pragma once

#include "ofxBaseGui.h"

class ofxValuePlotter: public ofxBaseGui {
public:
	struct Config: ofxBaseGui::Config{
		Config(){}
		Config(const ofxBaseGui::Config & c)
		:ofxBaseGui::Config(c){}

		float minValue=0;
		float maxValue=0;
		int plotSize=100;
	    int decimalPlace=3;
	};
    ofxValuePlotter(ofParameter<float> value, const Config & config);
    virtual ~ofxValuePlotter();

    void setDecimalPlace(int place);

    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
    virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

    virtual ofAbstractParameter & getParameter();

protected:

    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    void render();
    void generateDraw();
    void update(float & value);
    ofPath bg;
    ofVboMesh textMesh;
    std::vector<float> buffer;
    int plotSize;
    ofPath plot;
    float minVal, maxVal;
    bool autoscale;
    int decimalPlace;
    ofParameter<float> value;

};
