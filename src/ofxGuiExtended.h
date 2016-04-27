#pragma once

#include "ofxGuiElement.h"
#include "containers/ofxGuiGroup.h"
#include "containers/ofxGuiSliderGroup.h"
#include "containers/ofxGuiPanel.h"
#include "containers/ofxGuiTabs.h"
#include "controls/ofxGuiToggle.h"
#include "controls/ofxGuiSlider.h"
#include "controls/ofxGuiButton.h"
#include "controls/ofxGuiLabel.h"
#include "controls/ofxGuiValuePlotter.h"
#include "controls/ofxGuiFpsPlotter.h"
#include "controls/ofxGuiFunctionPlotter.h"
#include "controls/ofxGuiInputField.h"
#include "controls/ofxGuiGraphics.h"
#include "controls/ofxGuiZoomableGraphics.h"

#include "DOM/ofxDOM.h"

#include "view/ofxGuiDefaultConfig.h"
#include "view/ofxDOMFlexBoxLayout.h"
#include "view/ofxDOMBoxLayout.h"
#include "view/ofxDOMFloatingBoxLayout.h"
#include "view/JsonConfigParser.h"

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
		DOM::Document* getRoot();

		ofxGuiGroup* addGroup(const std::string& name="", const ofJson& config = ofJson());
		ofxGuiGroup* addGroup(const ofParameterGroup & parameters, const ofJson& config = ofJson());

		ofxGuiPanel* addPanel(const std::string& name="", const ofJson& config = ofJson());
		ofxGuiPanel* addPanel(const ofParameterGroup & parameters, const ofJson& config = ofJson());

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
		std::unique_ptr<DOM::Document> document;

		void setup();
		bool setup_done;

		ofJson rootGroupConfig(const ofJson& config = ofJson());

		ofxGuiGroup* defaultPanel = nullptr;
};

