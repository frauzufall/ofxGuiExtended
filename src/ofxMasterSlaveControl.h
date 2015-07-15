#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"

struct MasterControl {

    MasterControl(ofxBaseGui* _control) {
        control = _control;
        if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(control)) {
            min = slider->getMin();
            max = slider->getMax();
        }
        if(ofxSlider<int>* slider = dynamic_cast<ofxSlider<int>*>(control)) {
            min = slider->getMin();
            max = slider->getMax();
        }
    }

    ofxBaseGui* control = 0;
    bool isActive = false;
    bool slider = false;
    float min=1, max=1;
};

struct SlaveControl {

    ofxBaseGui* control = 0;
    bool isControlled = false;
    bool isListening = false;
    bool slider = false;
    float min=0, max=1;
    MasterControl* master = 0;
    ofColor defaultBackgroundColor;

    SlaveControl(ofxBaseGui* _control) {
        this->control = _control;
        defaultBackgroundColor = control->getBackgroundColor();
        if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(control)) {
            min = slider->getMin();
            max = slider->getMax();
        }
        if(ofxSlider<int>* slider = dynamic_cast<ofxSlider<int>*>(control)) {
            min = slider->getMin();
            max = slider->getMax();
        }
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
                toggle->addListener(this, &SlaveControl::valueChanged<bool>);
                isControlled = true;
                this->master = master;
            }
            if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(master->control)) {
                slider->addListener(this, &SlaveControl::valueChanged<float>);
                isControlled = true;
                this->master = master;
            }
            if(ofxSlider<int>* slider = dynamic_cast<ofxSlider<int>*>(master->control)) {
                slider->addListener(this, &SlaveControl::valueChanged<int>);
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
                toggle->removeListener(this, &SlaveControl::valueChanged<bool>);
            }
            if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(master->control)) {
                slider->removeListener(this, &SlaveControl::valueChanged<float>);
            }
            if(ofxSlider<int>* slider = dynamic_cast<ofxSlider<int>*>(master->control)) {
                slider->removeListener(this, &SlaveControl::valueChanged<int>);
            }
            isControlled = false;
            master = 0;
            control->setBackgroundColor(defaultBackgroundColor);
        }
    }

    template <class Tvalue>
    void valueChanged(Tvalue &value) {
        if(ofxSlider<float>* slider = dynamic_cast<ofxSlider<float>*>(control)) {
            *slider = ofMap(value, master->min, master->max,min,max);
        }
        else if(ofxSlider<int>* slider = dynamic_cast<ofxSlider<int>*>(control)) {
            *slider = ofMap(value, master->min, master->max,min,max);
        }
        else if(ofxToggle* toggle = dynamic_cast<ofxToggle*>(control)) {
            *toggle =  floor(ofMap(value, master->min, master->max,min,max)+0.5);
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
