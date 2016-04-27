#pragma once

#include "../ofxGuiElement.h"

class ofxGuiGraphics : public ofxGuiElement {
	public:

		ofxGuiGraphics(string canvasName="", const ofJson& config = ofJson());
		ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, const ofJson& config = ofJson());
		ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, float w, float h = 0);
		virtual ~ofxGuiGraphics();

		void setup(string canvasName = "", ofBaseDraws * graphics = 0, float w = 0, float h = 0);

		virtual float getMinWidth() override;
		virtual float getMinHeight() override;

		virtual void setGraphics(ofBaseDraws* graphics);

		virtual ofAbstractParameter & getParameter();

		static std::string getClassType();

	protected:

		virtual vector<std::string> getClassTypes() override;

		bool setValue(float mx, float my, bool bCheckBounds){
			return false;
		}
		virtual void render();
		virtual void generateDraw();
		ofPath bg;
		ofVboMesh textMesh;
		ofBaseDraws * graphics;
		ofParameter <std::string> label;
		bool _bLoaded;
		bool resizing = false;

};
