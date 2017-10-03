#pragma once

#include "../ofxGuiElement.h"

template<typename Type>
class ofxGuiInputField : public ofxGuiElement{
public:
	ofxGuiInputField();
	ofxGuiInputField(const ofJson &config);
	ofxGuiInputField(ofParameter<Type>& _val, const ofJson &config = ofJson());
	ofxGuiInputField(const std::string& _name, Type _val, Type _min, Type _max, const ofJson &config = ofJson());
	//TODO the setup non-ofParameter setup is a pain for the Type string (because of the forced min and max)

	~ofxGuiInputField();

	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	virtual float getMinWidth() override;
	virtual float getMinHeight() override;

	virtual bool mouseMoved(ofMouseEventArgs & args) override;
	virtual bool mousePressed(ofMouseEventArgs & args) override;
	virtual bool mouseDragged(ofMouseEventArgs & args) override;
	virtual bool mouseReleased(ofMouseEventArgs & args) override;
	virtual bool mouseScrolled(ofMouseEventArgs & args) override;

	void registerKeyEvents();
	void unregisterKeyEvents();

	virtual bool keyPressed(ofKeyEventArgs & args);
	virtual bool keyReleased(ofKeyEventArgs & args);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	Type operator=(Type v);
	operator const Type & ();

	ofAbstractParameter & getParameter() override;

	static std::string getClassType();

protected:

	void setup();

	virtual std::vector<std::string> getClassTypes() override;

	virtual void render() override;
	ofParameter<Type> value;
	bool bMousePressed;
	bool mouseInside;
	bool hasFocus;
	bool setValue(float mx, float my, bool bCheck) override;
	virtual void generateDraw() override;
	virtual void generateText();
	void valueChanged(Type & value);
	ofVboMesh textMesh;

	bool bRegisteredForKeyEvents;

	std::string input;
	float inputWidth;
	bool bChangedInternally;
	void parseInput();
	int insertKeystroke(const std::string & character);
	int insertAlphabetic(const std::string & character);

	int mousePressedPos; //set by mouse interaction
	bool hasSelectedText();

	float selectStartX, selectionWidth; //calculated from select indices
	int selectStartPos, selectEndPos;
	void calculateSelectionArea(int selectIdx1, int selectIdx2);

	virtual void drawSelectedArea();
	virtual void drawCursor();
	virtual void drawFocusedBB();
	virtual void drawMesh();

	int pressCounter;

	void leaveFocus();
};

typedef ofxGuiInputField<float> ofxGuiFloatInputField;
typedef ofxGuiInputField<int> ofxGuiIntInputField;
typedef ofxGuiInputField<std::string> ofxGuiTextField;
