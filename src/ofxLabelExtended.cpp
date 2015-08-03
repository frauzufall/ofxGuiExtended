#include "ofxLabelExtended.h"
using namespace std;

ofxLabelExtended::ofxLabelExtended(ofParameter<string> label, const Config & c)
:ofxLabel(label,c){
    showLabelName = c.showName;
}

void ofxLabelExtended::setShowLabelName(bool value){
	showLabelName = value;
}

bool ofxLabelExtended::isLabelNameShown() const{
    return showLabelName;
}

void ofxLabelExtended::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

    string name;
    if(showLabelName && !getName().empty()){
    	name = getName() + ": ";
    }

    textMesh = getTextMesh(name + (string)label, b.x + textPadding, b.y + b.height / 2 + 4);
}
