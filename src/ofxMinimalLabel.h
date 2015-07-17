/*
 * This class is actually not needed.
 * There could be an option in ofxLabel to hide/not render the parameter name
 * ofxLabel and setting the parameters name to "" or passing "" to setup produces the same output
 */

#pragma once
#include "ofxLabel.h"

class ofxMinimalLabel: public ofxLabel {
protected:
    void generateDraw();
};
