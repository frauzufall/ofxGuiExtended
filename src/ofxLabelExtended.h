#pragma once
#include "ofxLabel.h"

class ofxLabelExtended: public ofxLabel {
public:
    ofxLabelExtended(){}
    ofxLabelExtended(ofParameter<std::string> _label, float width = defaultWidth, float height = defaultHeight, bool _showLabelName = true);

    ofxLabelExtended * setup(ofParameter<std::string> _label, float width = defaultWidth, float height = defaultHeight, bool _showLabelName = true);
    ofxLabelExtended * setup(std::string labelName, std::string label, float width = defaultWidth, float height = defaultHeight, bool _showLabelName = true);

    ofxLabelExtended * setShowLabelName(bool value = true);

    bool isLabelNameShown();
protected:
    void generateDraw();
    bool showLabelName;
};
