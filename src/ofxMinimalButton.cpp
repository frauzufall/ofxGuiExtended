#include "ofxMinimalButton.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalButton::ofxMinimalButton(const Config & config)
:ofxMinimalToggle(ofParameter<bool>(config.name,false),config){
	value.setSerializable(false);
    if(b.width == 0) {
    	b.width = getTextWidth(config.name, config.shape.height);
    }
	bGuiActive = false;
    checkboxRect.set(1, 1, b.width - 2, b.height - 2);
    value.addListener(this,&ofxMinimalButton::valueChanged);
}

ofxMinimalButton::~ofxMinimalButton(){
    value.removeListener(this,&ofxMinimalButton::valueChanged);
}

bool ofxMinimalButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

void ofxMinimalButton::valueChanged(bool & value){
    ofxMinimalToggle::valueChanged(value);
    if(!value){
        ofNotifyEvent(triggerEvent, this);
    }
}
