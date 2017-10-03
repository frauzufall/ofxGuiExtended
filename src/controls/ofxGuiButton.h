#pragma once

#include "ofxGuiToggle.h"
#include "ofParameter.h"

class ofxGuiButton : public ofxGuiToggle{
public:

	ofxGuiButton();
	ofxGuiButton(const std::string& buttonName);
	ofxGuiButton(const std::string& buttonName, const ofJson & config);
	ofxGuiButton(ofParameter<void>& _val, const ofJson & config = ofJson());
	ofxGuiButton(ofParameter<bool>& _bVal, const ofJson & config = ofJson());

	~ofxGuiButton();

	virtual ofAbstractParameter & getParameter() override;

	virtual bool mouseReleased(ofMouseEventArgs & args) override;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
	voidvalue.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		voidvalue.removeListener(listener,method);
	}

	static std::string getClassType();

protected:

	void setup();

	virtual std::vector<std::string> getClassTypes() override;

	virtual void generateDraw() override;
	virtual bool setValue(float mx, float my, bool bCheck) override;
	ofParameter<void> voidvalue;
	bool useVoidValue {false};

};
