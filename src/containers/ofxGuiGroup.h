#pragma once
#include "ofxGuiContainer.h"


class ofxGuiGroupHeader : public ofxGuiElement {
	public:
		ofxGuiGroupHeader(const ofJson &config = ofJson());

		~ofxGuiGroupHeader();

		virtual bool mousePressed(ofMouseEventArgs & args) override;

		virtual float getMinWidth() override;
		virtual float getMinHeight() override;

		static std::string getClassType();

	protected:

		virtual std::vector<std::string> getClassTypes() override;

		virtual void generateDraw() override;
		virtual void render() override;
		ofVboMesh textMesh;

};


class ofxGuiGroup : public ofxGuiContainer {
	public:

		ofxGuiGroup();
		ofxGuiGroup(const std::string& collectionName);
		ofxGuiGroup(const std::string& collectionName, const ofJson & config);
		ofxGuiGroup(const ofParameterGroup & parameters, const ofJson &config = ofJson());
		ofxGuiGroup(const ofParameterGroup & parameters, const std::string& _filename, float x = 10, float y = 10);
		ofxGuiGroup(const std::string& collectionName, const std::string& _filename, float x = 10, float y = 10);

		virtual ~ofxGuiGroup();

		void setup();

		virtual void minimize();
		virtual void maximize();
		virtual void minimizeAll();
		virtual void maximizeAll();
		bool getMinimized();
		void toggleMinimize();

		void setShowHeader(bool show);
		ofxGuiElement* getHeader();

		virtual std::vector<ofxGuiElement*> getControls() override;

		static std::string getClassType();

	protected:

		virtual std::vector<std::string> getClassTypes() override;

		virtual void _setConfig(const ofJson & config) override;

		ofParameter<bool> minimized;
		ofParameter<bool> showHeader;
		ofxGuiElement* header;

		virtual void onHeaderVisibility(bool& showing);
		virtual void onHeaderHeight(float& height);

	private:
		float widthMaximized, heightMaximized;

};
