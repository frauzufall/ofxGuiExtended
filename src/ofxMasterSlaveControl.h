#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"

struct MasterControl {

    MasterControl(ofxBaseGui* _control) {
        control = _control;
    }

    ofxBaseGui* control = 0;
    bool isActive = false;
};

struct SlaveControl {

    ofxBaseGui* control = 0;
    bool isControlled = false;
    bool isListening = false;
    MasterControl* master = 0;
    ofColor defaultBackgroundColor;

    SlaveControl(ofxBaseGui* _control) {
        this->control = _control;
        defaultBackgroundColor = control->getBackgroundColor();
    }

    void setControlledBy(MasterControl *master) {
        bool setC = true,removeC = false;
        if(isControlled) {
            removeC = true;
        }
        if(this->master == master) {
            setC = false;
        }

        if(removeC) {
            removeControl();
        }
        if(setC) {
            if(ofxToggle* toggle = dynamic_cast<ofxToggle*>(master->control)) {
                toggle->addListener(this, &SlaveControl::valueChangedBool);
                isControlled = true;
                this->master = master;
            }
            if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(master->control)) {
                slider->addListener(this, &SlaveControl::valueChangedFloat);
                isControlled = true;
                this->master = master;
            }
            if(isControlled) {
                control->setBackgroundColor(master->control->getFillColor());
            }
        }
    }

    void removeControl() {
        if(master != 0) {
            if(ofxToggle* toggle = dynamic_cast<ofxToggle*>(master->control)) {
                toggle->removeListener(this, &SlaveControl::valueChangedBool);
            }
            if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(master->control)) {
                slider->removeListener(this, &SlaveControl::valueChangedFloat);
            }
            isControlled = false;
            master = 0;
            control->setBackgroundColor(defaultBackgroundColor);
        }
    }

    void valueChangedFloat(float &value) {
        if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(control)) {
            *slider = value;
        }
    }
    void valueChangedBool(bool &value) {
        if(ofxToggle* toggle = dynamic_cast<ofxToggle*>(control)) {
            *toggle = value;
        }
    }

};

class ofxMasterSlaveControl {

    public:
        static ofxMasterSlaveControl&	get();

        void draw();

        void addSlave(ofxBaseGui* control);
        void addMaster(ofxBaseGui* control);

        ofColor getHighlightColor();
        void setHighlightColor(ofColor);

        virtual bool mouseMoved(ofMouseEventArgs & args);
        virtual bool mousePressed(ofMouseEventArgs & args);
        virtual bool mouseDragged(ofMouseEventArgs & args);
        virtual bool mouseReleased(ofMouseEventArgs & args);
        virtual bool mouseScrolled(ofMouseEventArgs & args);
        virtual bool mouseEntered(ofMouseEventArgs & args){return false;}
        virtual bool mouseExited(ofMouseEventArgs & args){return false;}

    protected:
        ofxMasterSlaveControl();
        ~ofxMasterSlaveControl();

    private:

        void setActiveMaster(int master_index, bool active);

        vector<MasterControl*> masters;
        vector<SlaveControl*> slaves;

        MasterControl* activeMaster;

};
