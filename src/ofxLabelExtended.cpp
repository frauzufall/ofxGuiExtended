#include "ofxLabelExtended.h"
using namespace std;

ofxLabelExtended::ofxLabelExtended(ofParameter<string> _label, float width, float height, bool _showLabelName){
	setup(_label,width,height,_showLabelName);
}

ofxLabelExtended* ofxLabelExtended::setup(ofParameter<string> _label, float width, float height, bool _showLabelName){
    ofxLabel::setup(_label, width, height);
    showLabelName = _showLabelName;
    return this;
}

ofxLabelExtended* ofxLabelExtended::setup(string labelName, string _label, float width, float height, bool _showLabelName){
    label.set(labelName,_label);
    return setup(label,width,height,_showLabelName);
}
ofxLabelExtended * ofxLabelExtended::setShowLabelName(bool value){
    showLabelName = value;
    return this;
}
bool ofxLabelExtended::isLabelNameShown()
{
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
