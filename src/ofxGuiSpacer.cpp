#include "ofxGuiSpacer.h"
#include "ofxGuiGroupExtended.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiSpacer::ofxGuiSpacer(const Config & config)
:ofxBaseGui(config){
    spacing_size = config.size;
	if(layout==ofxBaseGui::Vertical) {
		cout << "layout vertical " << b.width << endl;
		b.height = spacing_size;
	} else {
		b.width = spacing_size;
	}
}

void ofxGuiSpacer::generateDraw(){
	cout << "gerating " << b.width << endl;
    bg.clear();
    bg.setFillColor(thisBackgroundColor);
    bg.setFilled(true);
    bg.rectangle(b.x,b.y,b.width,b.height);
}

void ofxGuiSpacer::render(){
    bg.draw();
}

ofAbstractParameter & ofxGuiSpacer::getParameter(){
    return parameter;
}
