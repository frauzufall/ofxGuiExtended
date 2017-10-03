#pragma once

#include "ofxGuiContainer.h"

class ofxGuiMenuLabel : public ofxGuiLabel {
	public:
		ofxGuiMenuLabel(const std::string& labelName, const std::string & label, const ofJson & config = ofJson());
		~ofxGuiMenuLabel(){}
	protected:
		virtual void generateDraw() override;
		virtual void render() override;
		virtual float getMinWidth() override;
		ofPath icon;
};

class ofxGuiSubmenuLabel : public ofxGuiMenuLabel {
	public:
		ofxGuiSubmenuLabel(const std::string& labelName, const std::string & label, const ofJson & config = ofJson());
		~ofxGuiSubmenuLabel(){}
	protected:
		virtual void generateDraw() override;
};

class ofxGuiMenu : public ofxGuiContainer {

	public:

		ofxGuiMenu();
		ofxGuiMenu(const std::string &collectionName, const ofJson & config = ofJson());
		ofxGuiMenu(std::string collectionName, std::string filename, float x = 10, float y = 10);
		ofxGuiMenu(const ofParameterGroup & parameters, const ofJson & config = ofJson());
		ofxGuiMenu(const ofParameterGroup & parameters, const std::string& filename, float x = 10, float y = 10);
		~ofxGuiMenu();

		void setup();

		static std::string getClassType();

		ofxGuiLabel* getMenuLabel();
		ofParameter<bool>& isRootMenu();

		virtual bool mouseMoved(ofMouseEventArgs & args) override;
		virtual bool mousePressed(ofMouseEventArgs & args) override;

	protected:

		void hideOtherMenusUp(DOM::Element* parent, ofxGuiMenu* exception);
		void hideOtherMenusDown(DOM::Element *parent, ofxGuiMenu* exception);
		void hideOtherMenusUpPress(ofxGuiContainer* parent, ofxGuiMenu* exception);
		void hideOtherMenusDownPress(ofxGuiContainer *parent, ofxGuiMenu* exception);
		bool isMouseOver(ofxGuiElement *el);

		virtual std::vector<std::string> getClassTypes() override;

		virtual void generateDraw() override;

		void onAdded(DOM::ElementEventArgs& args);

		ofParameter<bool> isOpen;

		ofxGuiLabel* label;

		bool onMenuLabel;
		bool rootMenu;
		bool changeLabelColor;

};

class ofxGuiRootMenu : public ofxGuiMenu {
	public:
		ofxGuiRootMenu(const std::string &collectionName, const ofJson & config = ofJson());
		~ofxGuiRootMenu();
};


template<typename ColorType>
class ofxGuiMenuColor_: public ofxGuiMenu{

public:
	ofxGuiMenuColor_(ofParameter<ofColor_<ColorType>> &value, const ofJson & config = ofJson());
	~ofxGuiMenuColor_();

	ofAbstractParameter & getParameter();

	ofColor_<ColorType> operator=(const ofColor_<ColorType> & v);
	operator const ofColor_<ColorType> & ();
protected:
	virtual void generateDraw() override;
	void changeSlider(const void * parameter, ColorType & value);
	void changeValue(ofColor_<ColorType> & value);
	ofParameter<ofColor_<ColorType> > value;
	bool sliderChanging;
	std::vector<std::string> names;
};

template<class VecType>
class ofxGuiMenuVec_ : public ofxGuiMenu {
public:

	ofxGuiMenuVec_(ofParameter<VecType> &value, const ofJson & config = ofJson());
	~ofxGuiMenuVec_();

	void setup();

	ofAbstractParameter & getParameter();

	VecType operator=(const VecType & v);
	operator const VecType & ();
	const VecType * operator->();
protected:
	void changeSlider(const void * parameter, float & value);
	void changeValue(VecType & value);
	ofParameter<VecType> value;
	bool sliderChanging;
	std::vector<std::string> names;
};

class ofxGuiMenuRectangle : public ofxGuiMenu {
public:

	ofxGuiMenuRectangle(ofParameter<ofRectangle> &value, const ofJson & config = ofJson());
	~ofxGuiMenuRectangle();

	void setup();

	ofAbstractParameter & getParameter();

	ofRectangle operator=(const ofRectangle & v);
	operator const ofRectangle & ();
	const ofRectangle * operator->();
protected:
	void changeSlider(const void * parameter, float & value);
	void changeValue(ofRectangle & value);
	ofParameter<ofRectangle> value;
	bool sliderChanging;
	std::vector<std::string> names;
};
