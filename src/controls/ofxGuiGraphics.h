#pragma once

#include "../ofxGuiElement.h"

class ofxGuiGraphics : public ofxGuiElement {
	public:

		ofxGuiGraphics(std::string canvasName="", const ofJson& config = ofJson());
		ofxGuiGraphics(std::string canvasName, ofBaseDraws * graphics, const ofJson& config = ofJson());
		ofxGuiGraphics(std::string canvasName, ofBaseDraws * graphics, float w, float h = 0);
		virtual ~ofxGuiGraphics();

		virtual float getMinWidth() override;
		virtual float getMinHeight() override;

		void setAutoHeight();
		void setAutoWidth();

		virtual void setGraphics(ofBaseDraws* graphics);

		virtual ofAbstractParameter & getParameter() override;

		static std::string getClassType();

	protected:

		void setup(std::string canvasName = "", float w = 0, float h = 0);

		virtual std::vector<std::string> getClassTypes() override;

		bool setValue(float mx, float my, bool bCheckBounds) override {
			return false;
		}
		virtual void render() override;
		virtual void generateDraw() override;

		virtual void onResize(DOM::ResizeEventArgs& args);

		ofPath bg;
		ofVboMesh textMesh;
		ofBaseDraws * graphics;
		ofParameter <std::string> label;
		bool _bLoaded;
		bool resizing = false;
				bool autoWidth, autoHeight;

};
