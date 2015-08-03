#pragma once

#include "ofxMinimalToggle.h"
#include "ofParameter.h"

class ofxMinimalButton : public ofxMinimalToggle {
public:
	struct Config: public ofxToggle::Config{
		Config(){}
		Config(const ofxToggle::Config & c)
		:ofxToggle::Config(c){}
		Config(const ofxBaseGui::Config & c)
		:ofxToggle::Config(c){}
		std::string name;
	};
    ofxMinimalButton(const Config & config);
    ~ofxMinimalButton();

	virtual bool mouseReleased(ofMouseEventArgs & args);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		ofAddListener(triggerEvent,listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		ofRemoveListener(triggerEvent,listener,method);
	}

private:

    ofEvent<void> triggerEvent;
    void valueChanged(bool & v);

};
