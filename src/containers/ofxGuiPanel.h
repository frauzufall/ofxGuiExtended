#pragma once

#include "ofxGuiGroup.h"
#include "ofImage.h"

class ofxGuiGroup;

class ofxGuiPanelHeader : public ofxGuiGroupHeader {

	public:

		ofxGuiPanelHeader();
		ofxGuiPanelHeader(const ofJson &config);

		~ofxGuiPanelHeader(){
		}

		virtual float getMinWidth() override;

		virtual bool mousePressed(ofMouseEventArgs & args) override;

		ofEvent<void> loadPressedE;
		ofEvent<void> savePressedE;

		static std::string getClassType();

	protected:

		virtual std::vector<std::string> getClassTypes() override;

		virtual void generateDraw() override;
		virtual void render() override;
		virtual void loadIcons();

		ofRectangle loadBox, saveBox;
		ofImage loadIcon;
		ofImage saveIcon;

};

class ofxGuiPanel : public ofxGuiGroup {

	public:

		ofxGuiPanel(const std::string& collectionName="");
		ofxGuiPanel(const std::string& collectionName, const ofJson & config);
		ofxGuiPanel(const ofParameterGroup & parameters, const ofJson & config = ofJson());
		ofxGuiPanel(const std::string& collectionName, const std::string& filename, float x = 10, float y = 10);
		ofxGuiPanel(const ofParameterGroup & parameters, const std::string& filename, float x = 10, float y = 10);

		~ofxGuiPanel();

		void setup();

		void onHeaderMove(DOM::MoveEventArgs& args);
		void onLoadPressed();
		void onSavePressed();

		static std::string getClassType();

	protected:
		virtual std::vector<std::string> getClassTypes() override;

	private:

		bool headerListenersLoaded;

};
