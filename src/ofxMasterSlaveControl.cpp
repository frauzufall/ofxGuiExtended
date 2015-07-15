#include "ofxMasterSlaveControl.h"

ofxMasterSlaveControl& ofxMasterSlaveControl::get() {
    static ofxMasterSlaveControl instance;
    return instance;
}

ofxMasterSlaveControl::ofxMasterSlaveControl() {

    slaves.clear();
    masters.clear();
    activeMaster = 0;

    ofRegisterMouseEvents(this, -1);

}
ofxMasterSlaveControl::~ofxMasterSlaveControl() {
    ofUnregisterMouseEvents(this);
}

void ofxMasterSlaveControl::draw() {

    if(activeMaster != 0) {
        ofSetColor(activeMaster->control->getFillColor());
        ofNoFill();
        ofSetLineWidth(3);
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

void ofxMasterSlaveControl::setActiveMaster(int master_index, bool active) {
    for(unsigned int i = 0; i < slaves.size(); i++) {
        slaves[i]->isListening = active;
    }
    masters[master_index]->isActive = active;
}

bool ofxMasterSlaveControl::mouseMoved(ofMouseEventArgs & args) {
    return false;
}

bool ofxMasterSlaveControl::mousePressed(ofMouseEventArgs & args) {

    bool onElement = false;
    for(unsigned int i = 0; i < slaves.size(); i++) {
        if(slaves.at(i)->control->getShape().inside(args.x, args.y)) {
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
        if(masters.at(i)->control->getShape().inside(args.x, args.y)) {
            onElement = true;
            if(masters.at(i)->isActive) {
                masters.at(i)->isActive = false;
                activeMaster = 0;
            }
            else {
                masters.at(i)->isActive = true;
                if(activeMaster != 0) {
                    activeMaster->isActive = false;
                }
                activeMaster = masters.at(i);
            }
        }
    }
    if(!onElement) {
        if(activeMaster != 0) {
            activeMaster->isActive = false;
        }
        activeMaster = 0;
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

