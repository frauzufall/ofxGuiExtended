#pragma once

#include "ofParameter.h"
#include "../ofxGuiElement.h"

class ofxGuiToggleType{
	public:
	enum Type {
		/// \brief Shows toggle as checkbox (default).
		CHECKBOX,
		/// \brief Shows toggle as radio toggle.
		RADIO,
		/// \brief Uses the whole element as toggle.
		FULLSIZE
	};
};

class ofxGuiToggle : public ofxGuiElement{
public:

	ofxGuiToggle();
	ofxGuiToggle(const std::string& toggleName);
	ofxGuiToggle(const std::string& toggleName, const ofJson & config);
	ofxGuiToggle(ofParameter<bool>& _bVal, const ofJson & config = ofJson());
	ofxGuiToggle(const std::string& toggleName, bool _bVal, const ofJson & config = ofJson());

	~ofxGuiToggle();

	void setType(const std::string &type);
	void setType(const ofxGuiToggleType::Type &type);
	ofxGuiToggleType::Type getType();

	virtual float getMinWidth() override;
	virtual float getMinHeight() override;

	virtual bool mousePressed(ofMouseEventArgs & args) override;
	virtual bool mouseReleased(ofMouseEventArgs & args) override;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	bool operator=(bool v);
	operator const bool & ();

	virtual ofAbstractParameter & getParameter() override;

	static std::string getClassType();

protected:

	void setup();

	virtual std::vector<std::string> getClassTypes() override;

	virtual void _setConfig(const ofJson & config) override;
	virtual void render() override;
	ofRectangle checkboxRect;
	ofParameter<bool> value;

	ofParameter<ofxGuiToggleType::Type> type;
	bool hasFocus;

	virtual bool setValue(float mx, float my, bool bCheck) override;
	virtual void generateDraw() override;
	void valueChanged(bool & value);
	ofVboMesh textMesh;
};
