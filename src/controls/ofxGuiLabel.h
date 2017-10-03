#pragma once

#include "../ofxGuiElement.h"
#include "ofParameter.h"

template<typename Type>
class ofxGuiValueLabel: public ofxGuiElement {
public:

	ofxGuiValueLabel();
	ofxGuiValueLabel(ofParameter<Type>& _label, const ofJson & config = ofJson());
	ofxGuiValueLabel(const std::string& labelName, const ofJson & config = ofJson());
	ofxGuiValueLabel(ofParameter<Type>& _label, float width, float height);
	ofxGuiValueLabel(const std::string& labelName, const Type & label, const ofJson & config = ofJson());
	ofxGuiValueLabel(const std::string& labelName, const Type & label, float width, float height);

	virtual ~ofxGuiValueLabel();

	virtual float getMinWidth() override;
	virtual float getMinHeight() override;


	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		label.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		label.removeListener(listener,method);
	}


	Type operator=(Type v) { label = v; return v; }
	operator const Type & () { return label; }

	ofAbstractParameter & getParameter() override;

	static std::string getClassType();

protected:

	void setup();

	virtual std::vector<std::string> getClassTypes() override;

	virtual void render() override;
	ofParameter<Type> label;
	virtual void generateDraw() override;
	void valueChanged(Type & value);
	bool setValue(float mx, float my){return false;}
	ofVboMesh textMesh;
};

typedef ofxGuiValueLabel<std::string> ofxGuiLabel;
typedef ofxGuiValueLabel<int> ofxGuiIntLabel;
typedef ofxGuiValueLabel<float> ofxGuiFloatLabel;
typedef ofxGuiValueLabel<bool> ofxGuiBoolLabel;
