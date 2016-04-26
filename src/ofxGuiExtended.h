#pragma once

#include "ofxToggle.h"
#include "ofxSlider.h"
#include "ofxSliderGroup.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofxGuiTabs.h"
#include "ofxValuePlotter.h"
#include "ofxFpsPlotter.h"
#include "ofxFunctionPlotter.h"
#include "ofxInputField.h"
#include "ofxGuiGraphics.h"
#include "ofxGuiZoomableGraphics.h"

#include "ofxGuiDefaultConfig.h"

#include "ofxDOM.h"

void ofxGuiSetFont(const string & fontPath,int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, int dpi=0);
void ofxGuiSetBitmapFont();

void ofxGuiSetHeaderColor(const ofColor & color);
void ofxGuiSetBackgroundColor(const ofColor & color);
void ofxGuiSetBorderColor(const ofColor & color);
void ofxGuiSetTextColor(const ofColor & color);
void ofxGuiSetFillColor(const ofColor & color);

void ofxGuiSetTextPadding(int padding);
void ofxGuiSetDefaultWidth(int width);
void ofxGuiSetDefaultHeight(int height);

class ofxGui {
	public:
		ofxGui();
		~ofxGui();
		Document* getRoot();

		ofxGuiGroup* addGroup(const std::string& name="", const ofJson& config = ofJson());
		ofxGuiGroup* addGroup(const ofParameterGroup & parameters, const ofJson& config = ofJson());

		ofxPanel* addPanel(const std::string& name="", const ofJson& config = ofJson());
		ofxPanel* addPanel(const ofParameterGroup & parameters, const ofJson& config = ofJson());

		ofxGuiTabs* addTabs(const std::string& name="", const ofJson& config = ofJson());

		template<typename T>
		void add(ofParameter<T>& parameter){
			if(!defaultPanel){
				defaultPanel = addPanel();
			}
			defaultPanel->add(parameter);
		}

		template<typename T, typename... Args>
		void add(ofParameter<T>& parameter, Args... args) {
			add(parameter);
			add(args...) ;
		}

	private:
		std::unique_ptr<ofx::DOM::Document> document;

		void setup();
		bool setup_done;

		ofJson rootGroupConfig(const ofJson& config);

		ofxGuiGroup* defaultPanel = nullptr;
};

