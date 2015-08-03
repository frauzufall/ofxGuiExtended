#pragma once

#include "ofxBaseGui.h"

class ofxGuiSpacer : public ofxBaseGui {
public:
	struct Config: public ofxBaseGui::Config{
		Config(){}
		Config(float size=10)
		:size(size){

		}
		float size = 10;
	};
    ofxGuiSpacer(const Config & config);
    virtual ~ofxGuiSpacer() {}

    virtual ofAbstractParameter & getParameter();

    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
    virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

protected:
    virtual void render();
    virtual bool setValue(float mx, float my, bool bCheckBounds){return false;}
    virtual void generateDraw();

    ofPath bg;
    float spacing_size;

    // not needed, but has be be there
    ofParameter<bool> parameter;

};
