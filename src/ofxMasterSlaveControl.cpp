#include "ofxMasterSlaveControl.h"

ofxMasterSlaveControl::ofxMasterSlaveControl() {

	slaves.clear();
	masters.clear();
	activeMaster = 0;

	ofRegisterMouseEvents(this, -1);

}
ofxMasterSlaveControl::~ofxMasterSlaveControl() {
	ofUnregisterMouseEvents(this, -1);
}

void ofxMasterSlaveControl::draw() {

	if(activeMaster != 0) {
		ofSetColor(activeMaster->control->getFillColor(), 133);
		ofFill();
		//ofSetLineWidth(3);
		for(unsigned int i = 0; i < slaves.size(); i++) {
			ofDrawRectangle(slaves.at(i)->control->getShape());
		}
		ofDrawRectangle(activeMaster->control->getShape());
	}
}

void ofxMasterSlaveControl::addMaster(ofxBaseGui *control) {
	masters.push_back(new MasterControl(control));
}

void ofxMasterSlaveControl::addSlave(ofxBaseGui *control) {
	slaves.push_back(new SlaveControl(control));
}

void ofxMasterSlaveControl::setActiveMaster(MasterControl* master) {

	if(master){
		if(activeMaster){
			if(activeMaster != master){
				activeMaster->isActive = false;
			}
		}
		activeMaster = master;
		master->isActive = true;
		for(unsigned int i = 0; i < slaves.size(); i++) {
			slaves[i]->setListening(true, master);
		}
	}else {
		if(activeMaster){
			activeMaster->isActive = false;
			activeMaster = nullptr;
		}
		for(unsigned int i = 0; i < slaves.size(); i++) {
			slaves[i]->setListening(false, master);
		}
	}
}

bool ofxMasterSlaveControl::mouseMoved(ofMouseEventArgs & args) {
	return false;
}

bool ofxMasterSlaveControl::mousePressed(ofMouseEventArgs & args) {

	bool onElement = false;
	for(unsigned int i = 0; i < slaves.size(); i++) {
		if(slaves.at(i)->control->isMouseOver()) {
			onElement = true;
			if(activeMaster != 0) {
				slaves.at(i)->setControlledBy(activeMaster);
			}
			else {
				slaves.at(i)->removeControl();
			}
		}
	}
	for(unsigned int i = 0; i < masters.size(); i++) {
		if(masters.at(i)->control->isMouseOver()) {
			onElement = true;
			if(masters.at(i)->isActive) {
				setActiveMaster(nullptr);
			}
			else {
				setActiveMaster(masters.at(i));
			}
		}
	}
	if(!onElement) {
		setActiveMaster(nullptr);
	}
	return false;
}

bool ofxMasterSlaveControl::mouseDragged(ofMouseEventArgs & args) {
	return false;
}

bool ofxMasterSlaveControl::mouseReleased(ofMouseEventArgs & args) {
	return false;
}

bool ofxMasterSlaveControl::mouseScrolled(ofMouseEventArgs & args) {
	return false;
}

