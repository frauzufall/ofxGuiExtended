#pragma once

#include "ofxGuiContainer.h"

class ofxGuiTabs : public ofxGuiContainer {

	public:

		ofxGuiTabs();
		ofxGuiTabs(const std::string &collectionName, const ofJson & config = ofJson());
		ofxGuiTabs(std::string collectionName, std::string filename, float x = 10, float y = 10);
		~ofxGuiTabs();

		void setup();

		using ofxGuiContainer::add;

		template <typename GuiType>
		GuiType* add(std::unique_ptr<GuiType> element);

		virtual void clear() override;

		void setActiveTab(int index);
		ofParameter<int> &getActiveTabIndex();
		Element * getActiveTab();

		void setTabHeight(int h);
		void setTabWidth(int w);

		static std::string getClassType();

	protected:

		void _setActiveTab(int &index);

		virtual std::vector<std::string> getClassTypes() override;

		virtual void generateDraw() override;

		void onChildAdd(DOM::ElementEventArgs& args);

		ofParameter<float> tabWidth;
		ofParameter<float> tabHeight;

		ofxGuiContainer * tabs;
		std::vector<Element*> pages;
		Element * activePage;
		ofxGuiElement * activeToggle;


	private:
};
