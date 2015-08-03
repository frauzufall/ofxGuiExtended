#pragma once
#include "ofxLabel.h"

class ofxLabelExtended: public ofxLabel {
public:
	struct Config: public ofxLabel::Config{
		Config(){}
		Config(const ofxLabel::Config & c)
		:ofxLabel::Config(c){}
		Config(const ofxBaseGui::Config & c)
		:ofxLabel::Config(c){}
		bool showName = true;
	};
    ofxLabelExtended(ofParameter<std::string> _label, const Config & c);

    void setShowLabelName(bool value = true);
    bool isLabelNameShown() const;
protected:
    void generateDraw();
    bool showLabelName;
};
